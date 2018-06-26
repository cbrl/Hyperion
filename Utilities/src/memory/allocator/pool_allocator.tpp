template<typename T>
PoolAllocator<T>::PoolAllocator(size_t memory_size) : BlockAllocator<T>(memory_size) {

	static_assert(sizeof(T) >= 8 && "PoolAllocator chunk size must be 8 or greater.");
	assert(memory_size % sizeof(T) == 0 && "PoolAllocator memory size must be a multiple of the chunk size.");

	reset();
}


template<typename T>
void PoolAllocator<T>::reset() {
	this->memory_used = 0;

	const size_t padding = AllocatorUtil::CalculatePadding(reinterpret_cast<uintptr>(this->start_ptr), alignof(T));

	const size_t num_chunks = (this->memory_size - padding) / sizeof(T);

	const uintptr aligned_start = reinterpret_cast<uintptr>(this->start_ptr) + padding;

	for (size_t i = 0; i < num_chunks; ++i) {
		uintptr addr = aligned_start + (i * (sizeof(T)));
		free_list.push_front(reinterpret_cast<node*>(addr));
	}
}


template<typename T>
T* PoolAllocator<T>::allocate() {

	node* free_addr = free_list.pop_front();

	if (free_addr == nullptr) {
		return nullptr;
	}

	this->memory_used += sizeof(T);

	return reinterpret_cast<T*>(free_addr);
}


template<typename T>
void PoolAllocator<T>::deallocate(void* ptr) {

	free_list.push_front(static_cast<node*>(ptr));

	this->memory_used -= sizeof(T);
}
