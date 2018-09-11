template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::ResourcePool() : count(0) {

	// Create the first chunk
	memory_chunks.push_front(std::make_unique<Chunk>(chunk_objects * sizeof(DataT)));
}


template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
void* ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::allocate() {

	DataT* object = nullptr;

	// Find a chunk with free space and create the object
	for (auto& chunk : memory_chunks) {

		// Skip this chunk if it is full
		if (chunk->objects.size() >= (chunk->memory_size / sizeof(DataT)))
			continue;

		// Create the object
		object = chunk->allocator->allocate();

		if (object) {
			chunk->objects.push_back(object);
			break;
		}
	}

	// Create a new chunk if all others are full
	if (!object) {
		auto& chunk = createChunk();
		object = chunk->allocator->allocate();

		assert(object != nullptr && "ResourcePool::allocateObject() - Unable to create object.");

		chunk->objects.push_back(object);
	}

	++count;
	return reinterpret_cast<void*>(object);
}


template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
template<typename... ArgsT>
DataT* ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::construct(ArgsT&&... args) {

	void*  memory = allocate();
	DataT* object = new(memory) DataT(std::forward<ArgsT>(args)...);

	return object;
}


template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
void ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::deallocate(DataT* object) {

	const uintptr addr = reinterpret_cast<uintptr>(object);

	for (auto& chunk : memory_chunks) {

		if (addr >= chunk->start_addr &&
		    addr < (chunk->start_addr + chunk->memory_size)) {

			object->~DataT();

			chunk->objects.remove(object);
			chunk->allocator->deallocate(object);

			--count;
			return;
		}
	}

	assert(false && "Failed to delete object. Possible corrupted memory.");
}


template<typename DataT, size_t InitialChunkObjects, size_t MaxChunkObjects>
unique_ptr<typename ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>::Chunk>&
ResourcePool<DataT, InitialChunkObjects, MaxChunkObjects>
::createChunk() {
	
	// Increase the allocation size, capped at MaxChunkObjects
	chunk_objects = std::min(MaxChunkObjects, chunk_objects << 1);

	// Create a new chunk with the new allocation size
	memory_chunks.push_front(std::make_unique<Chunk>(chunk_objects * sizeof(DataT)));

	return memory_chunks.front();
}
