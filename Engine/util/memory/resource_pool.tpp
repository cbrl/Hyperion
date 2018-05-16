
template<typename DataT, size_t max_objs_per_chunk>
ResourcePool<DataT, max_objs_per_chunk>::ResourcePool() : count(0) {

	PoolAllocator<DataT>* alloc = new PoolAllocator<DataT>(alloc_size);

	// Create the first chunk
	memory_chunks.push_front(new Chunk(alloc));
}


template<typename DataT, size_t max_objs_per_chunk>
ResourcePool<DataT, max_objs_per_chunk>::~ResourcePool() {

	for (auto chunk : memory_chunks) {

		// Delete every object in the list
		for (auto obj : chunk->objects) {
			static_cast<DataT*>(obj)->~DataT();
		}

		// Clear the list
		chunk->objects.clear();

		// Delete the allocator
		delete chunk->allocator;
		chunk->allocator = nullptr;

		// Delete the chunk
		delete chunk;
		chunk = nullptr;
	}

	count = 0;
}


template<typename DataT, size_t max_objs_per_chunk>
void* ResourcePool<DataT, max_objs_per_chunk>::AllocateObject() {

	DataT* object = nullptr;

	// Find a chunk with free space and create the object
	for (auto chunk : memory_chunks) {

		if (chunk->objects.size() >= max_objs_per_chunk) continue;

		object = chunk->allocator->AllocateCast();
		
		if (object) {
			chunk->objects.push_back(object);
			break;
		}
	}

	// Create a new chunk if the others are full
	if (!object) {
		PoolAllocator<DataT>* alloc = new PoolAllocator<DataT>(alloc_size);

		Chunk* chunk = new Chunk(alloc);
		chunk->objects.clear();
		memory_chunks.push_front(chunk);

		object = chunk->allocator->AllocateCast();

		if (object == nullptr) {
			FILE_LOG(logERROR) << "ResourcePool::AllocateObject() - Unable to create object.";
		}

		assert(object != nullptr && "ResourcePool::AllocateObject() - Unable to create object.");

		chunk->objects.push_back(object);
	}

	++count;
	return reinterpret_cast<void*>(object);
}


template<typename DataT, size_t max_objs_per_chunk>
void ResourcePool<DataT, max_objs_per_chunk>::DestroyObject(void* object) {

	uintptr addr = reinterpret_cast<uintptr>(object);

	for (auto chunk : memory_chunks) {

		if (addr >= chunk->start_addr &&
			addr < (chunk->start_addr + alloc_size)) {

			static_cast<DataT*>(object)->~DataT();

			chunk->objects.remove(static_cast<DataT*>(object));
			chunk->allocator->Free(object);

			--count;
			return;
		}
	}

	FILE_LOG(logERROR) << "Failed to delete object. Possible corrupted memory.";
	assert(false && "Failed to delete object. Possible corrupted memory.");
}
