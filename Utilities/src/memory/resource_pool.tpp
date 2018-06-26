template<typename DataT, size_t max_objs_per_chunk>
ResourcePool<DataT, max_objs_per_chunk>::ResourcePool() : count(0) {

	// Create the first chunk
	memory_chunks.push_front(make_unique<Chunk>(alloc_size));
}


template<typename DataT, size_t max_objs_per_chunk>
void* ResourcePool<DataT, max_objs_per_chunk>::allocateObject() {

	DataT* object = nullptr;

	// Find a chunk with free space and create the object
	for (auto& chunk : memory_chunks) {

		if (chunk->objects.size() >= max_objs_per_chunk) continue;

		object = chunk->allocator->allocate();

		if (object) {
			chunk->objects.push_back(object);
			break;
		}
	}

	// Create a new chunk if the others are full
	if (!object) {
		memory_chunks.push_front(make_unique<Chunk>(alloc_size));

		auto& chunk = memory_chunks.front();

		object = chunk->allocator->allocate();

		assert(object != nullptr && "ResourcePool::allocateObject() - Unable to create object.");

		chunk->objects.push_back(object);
	}

	++count;
	return reinterpret_cast<void*>(object);
}


template<typename DataT, size_t max_objs_per_chunk>
void ResourcePool<DataT, max_objs_per_chunk>::destroyObject(void* object) {

	const uintptr addr = reinterpret_cast<uintptr>(object);

	for (auto& chunk : memory_chunks) {

		if (addr >= chunk->start_addr &&
		    addr < (chunk->start_addr + alloc_size)) {

			static_cast<DataT*>(object)->~DataT();

			chunk->objects.remove(static_cast<DataT*>(object));
			chunk->allocator->deallocate(object);

			--count;
			return;
		}
	}

	assert(false && "Failed to delete object. Possible corrupted memory.");
}
