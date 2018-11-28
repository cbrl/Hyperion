template<typename HandleT, typename DataT, size_t ChunkSize>
bool HandleTable<HandleT, DataT, ChunkSize>::allocateChunk() {

	if (table.size() >= HandleT::index_max) {
		Logger::log(LogLevel::err, "Max handle table size reached ({})", HandleT::index_max);
		assert(false && "Max handle table size reached");
		return false;
	}

	size_t size = std::min(table.size() + ChunkSize, HandleT::index_max);
	table.resize(size);
	return true;
}


template<typename HandleT, typename DataT, size_t ChunkSize>
HandleT HandleTable<HandleT, DataT, ChunkSize>::createHandle(DataT* object) {

	size_t i = 0;

	// Find the next free space and create the object/handle
	for (; i < table.size(); ++i) {

		auto& h = table[i];

		if (h.second == nullptr) {
			h.second = object;

			if (h.first > HandleT::counter_max) h.first = 0;
			else h.first += 1;

			return HandleT(i, h.first);
		}
	}

	// If this point is reached, then the table needs to
	// be expanded before a new handle is created.

	// Try to increase table size
	if (allocateChunk()) {

		// Create the object/handle
		table[i].first = 1;
		table[i].second = object;

		return HandleT(i, table[i].first);
	}

	// Return an invalid handle if one couldn't be created
	return HandleT::invalid_handle;
}


template<typename HandleT, typename DataT, size_t ChunkSize>
void HandleTable<HandleT, DataT, ChunkSize>::releaseHandle(HandleT handle) {

	if (!validHandle(handle)) {
		Logger::log(LogLevel::err, "Invalid handle specified for release (index: {}, counter: {})", handle.index, handle.counter);
		assert(false && "Invalid handle specified for release");
		return;
	}

	table[handle.index].second = nullptr;
}


template<typename HandleT, typename DataT, size_t ChunkSize>
bool HandleTable<HandleT, DataT, ChunkSize>::validHandle(const HandleT& handle) const {

	if (handle == HandleT::invalid_handle
		|| handle.index > table.size()) {
		return false;
	}

	const auto& entry = table[handle.index];
	return (entry.first == handle.counter && entry.second != nullptr);
}