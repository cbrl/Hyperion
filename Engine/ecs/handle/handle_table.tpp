
template<typename HandleT, typename DataT, size_t chunk_size>
void HandleTable<HandleT, DataT, chunk_size>::AllocateChunk() {

	ThrowIfFailed((handles.size() < HandleT::index_max), "Max handle table size exceeded.");

	size_t size = std::min(handles.size() + chunk_size, HandleT::index_max);

	handles.resize(size);
}


template<typename HandleT, typename DataT, size_t chunk_size>
HandleT HandleTable<HandleT, DataT, chunk_size>::CreateHandle(DataT* object) {

	size_t i = 0;

	// Find the next free space and create the object/handle
	for (; i < handles.size(); ++i) {

		auto& h = handles[i];

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
	AllocateChunk();

	// Create the object/handle
	handles[i].first = 1;
	handles[i].second = object;

	return HandleT(i, handles[i].first);
}


template<typename HandleT, typename DataT, size_t chunk_size>
void HandleTable<HandleT, DataT, chunk_size>::DestroyHandle(const HandleT& handle) {

	LogIfFailed((handle.index < handles.size()) && (handle.counter == handles[handle.index].first),
				"Invalid handle specified for release.");

	handles[handle.index].second = nullptr;
}