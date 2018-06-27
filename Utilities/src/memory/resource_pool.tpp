template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::ResourcePool() : count(0) {

	// Create the first chunk
	memory_chunks.push_front(make_unique<Chunk>(chunk_objects * sizeof(DataT)));
}


template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
void* ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::allocateObject() {

	DataT* object = nullptr;

	// Find a chunk with free space and create the object
	for (auto& chunk : memory_chunks) {

		// Check if the number of objects allocated is less than the max
		if (chunk->objects.size() >= (chunk->memory_size / sizeof(DataT))) continue;

		object = chunk->allocator->allocate();

		if (object) {
			chunk->objects.push_back(object);
			break;
		}
	}

	// Create a new chunk if the others are full
	if (!object) {
		createChunk();

		auto& chunk = memory_chunks.front();
		object = chunk->allocator->allocate();

		assert(object != nullptr && "ResourcePool::allocateObject() - Unable to create object.");

		chunk->objects.push_back(object);
	}

	++count;
	return reinterpret_cast<void*>(object);
}


template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
void ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::destroyObject(void* object) {

	const uintptr addr = reinterpret_cast<uintptr>(object);

	for (auto& chunk : memory_chunks) {

		if (addr >= chunk->start_addr &&
		    addr < (chunk->start_addr + chunk->memory_size)) {

			static_cast<DataT*>(object)->~DataT();

			chunk->objects.remove(static_cast<DataT*>(object));
			chunk->allocator->deallocate(object);

			--count;
			return;
		}
	}

	assert(false && "Failed to delete object. Possible corrupted memory.");
}


template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
void ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::createChunk() {
	
	// Increase the allocation size, capped at MaxChunkObjects
	chunk_objects = std::min(MaxChunkObjects, chunk_objects << 1);

	// Create a new chunk with the new allocation size
	memory_chunks.push_front(make_unique<Chunk>(chunk_objects * sizeof(DataT)));
}