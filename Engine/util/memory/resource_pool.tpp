
template<typename DataT, size_t max_objs_per_chunk>
ResourcePool<DataT, max_objs_per_chunk>::ResourcePool() {

	PoolAllocator* alloc = new PoolAllocator(alloc_size, sizeof(DataT), alignof(DataT));

	// Create the first chunk
	memory_chunks.push_front(new Chunk(alloc));
}


template<typename DataT, size_t max_objs_per_chunk>
ResourcePool<DataT, max_objs_per_chunk>::~ResourcePool() {

	for (auto chunk : memory_chunks) {

		// Delete every object in the list
		for (auto obj : chunk->objects) {
			delete obj;
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
}


template<typename DataT, size_t max_objs_per_chunk>
void* ResourcePool<DataT, max_objs_per_chunk>::CreateObject() {

	void* object = nullptr;

	// Find a chunk with free space and create the object
	for (auto chunk : memory_chunks) {

		if (chunk->objects.size() >= max_objs_per_chunk) continue;

		object = chunk->allocator->Allocate(sizeof(DataT), alignof(DataT));
		
		if (object) {
			chunk->objects.push_back(reinterpret_cast<DataT*>(object));
			break;
		}
	}

	// Create a new chunk if the others are full
	if (!object) {

		PoolAllocator* alloc = new PoolAllocator(alloc_size, sizeof(DataT), alignof(DataT));

		Chunk* chunk = new Chunk(alloc);
		chunk->objects.clear();

		memory_chunks.push_front(chunk);

		object = chunk->allocator->Allocate(sizeof(DataT), alignof(DataT));

		assert(object != nullptr && "Unable to create object.");
		chunk->objects.push_back(reinterpret_cast<DataT*>(object));
	}

	return object;
}


template<typename DataT, size_t max_objs_per_chunk>
void ResourcePool<DataT, max_objs_per_chunk>::DestroyObject(void* object) {

	uintptr addr = reinterpret_cast<uintptr>(object);

	for (auto chunk : memory_chunks) {

		if (addr >= chunk->start_addr &&
			addr < (chunk->start_addr + alloc_size)) {

			reinterpret_cast<DataT*>(object)->~DataT();

			chunk->objects.remove(reinterpret_cast<DataT*>(object));
			chunk->allocator->Free(object);

			return;
		}
	}

	assert(false && "Failed to delete object. Possible corrupted memory.");
}
