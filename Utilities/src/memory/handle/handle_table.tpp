template<typename HandleT, typename DataT, size_t ChunkSize>
void HandleTable<HandleT, DataT, ChunkSize>::allocateChunk() {

	assert((handle_table.size() < HandleT::index_max) && "Max handle table size exceeded.");

	size_t size = std::min(handle_table.size() + ChunkSize, HandleT::index_max);

	handle_table.resize(size);
}


template<typename HandleT, typename DataT, size_t ChunkSize>
HandleT HandleTable<HandleT, DataT, ChunkSize>::createHandle(DataT* object) {

	size_t i = 0;

	// Find the next free space and create the object/handle
	for (; i < handle_table.size(); ++i) {

		auto& h = handle_table[i];

		if (h.second == nullptr) {
			h.second = object;

			if (h.first > HandleT::counter_max) h.first = 0;
			else h.first += 1;

			return HandleT(i, h.first);
		}
	}

	// If this point is reached, then the table needs to
	// be expanded before a new handle is created.

	// Increase table size
	allocateChunk();

	// Create the object/handle
	handle_table[i].first = 1;
	handle_table[i].second = object;

	return HandleT(i, handle_table[i].first);
}


template<typename HandleT, typename DataT, size_t ChunkSize>
void HandleTable<HandleT, DataT, ChunkSize>::releaseHandle(HandleT handle) {
	assert(validHandle(handle) && "Invalid handle specified for release");

	handle_table[handle.index].second = nullptr;
}


template<typename HandleT, typename DataT, size_t ChunkSize>
bool HandleTable<HandleT, DataT, ChunkSize>::validHandle(const HandleT& handle) const {
	assert(handle != HandleT::invalid_handle && "Invalid handle specified");
	assert(handle.index < handle_table.size() && "Invalid handle specified. Index out of range.");

	return handle.counter == handle_table[handle.index].first;
}