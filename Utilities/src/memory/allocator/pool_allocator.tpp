template<typename DataT>
PoolAllocator<DataT>::PoolAllocator(const size_t memory_size, const size_t chunk_size, const size_t alignment)
	: Allocator(memory_size)
	, chunk_size(chunk_size)
	, align(alignment) {

	assert(chunk_size >= 8 && "PoolAllocator chunk size must be 8 or greater.");
	assert(memory_size % chunk_size == 0 && "PoolAllocator memory size must be a multiple of the chunk size.");

	start_ptr = malloc(memory_size);
	reset();
}


template<typename DataT>
PoolAllocator<DataT>::~PoolAllocator() {
	free(start_ptr);
	start_ptr = nullptr;
}


template<typename DataT>
void PoolAllocator<DataT>::reset() {
	memory_used = 0;
	peak = 0;

	const size_t padding = AllocatorUtil::CalculatePadding(reinterpret_cast<uintptr>(start_ptr), this->align);

	const size_t num_chunks = (memory_size - padding) / chunk_size;

	const uintptr aligned_start = reinterpret_cast<uintptr>(start_ptr) + padding;

	for (size_t i = 0; i < num_chunks; ++i) {
		uintptr addr = aligned_start + (i * (chunk_size));
		free_list.push_front(reinterpret_cast<node*>(addr));
	}
}


template<typename DataT>
void* PoolAllocator<DataT>::allocate(const size_t size, const size_t alignment) {

	// Ensure the the arguments are valid.
	assert(size == chunk_size && "PoolAllocator allocation size is not equal to chunk size.");
	assert(alignment == this->align && "PoolAllocator allocation alignment is not equal to alignment specified on creation.");

	node* free_addr = free_list.pop_front();

	if (free_addr == nullptr) {
		return nullptr;
	}

	this->memory_used += chunk_size;
	this->peak = std::max(peak, memory_used);

	return static_cast<void*>(free_addr);
}

template<typename DataT>
DataT* PoolAllocator<DataT>::allocateCast() {
	return static_cast<DataT*>(allocate());
}


template<typename DataT>
void PoolAllocator<DataT>::freeMemory(void* ptr) {

	free_list.push_front(static_cast<node*>(ptr));

	this->memory_used -= chunk_size;
}

template<typename DataT>
void PoolAllocator<DataT>::freeMemory(DataT* ptr) {

	this->freeMemory(reinterpret_cast<void*>(ptr));
}
