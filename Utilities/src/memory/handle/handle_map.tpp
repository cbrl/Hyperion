template <typename HandleT, typename DataT, size_t chunk_size>
DataT HandleMap<HandleT, DataT, chunk_size>::operator[](HandleT handle) {
	bool valid = isValid(handle);
	if (!valid) {
		Logger::log(LogLevel::err, "Invalid handle specified (index: {}, counter: {})", handle.index, handle.counter);
		assert(false && "HandleMap::operator[] - Invalid handle specified");
	}
	return valid ? table[handle.index].second : nullptr;
}


template<typename HandleT, typename DataT, size_t chunk_size>
bool HandleMap<HandleT, DataT, chunk_size>::allocateChunk() {

	if (table.size() >= HandleT::index_max) {
		Logger::log(LogLevel::err, "Max handle table size reached ({})", HandleT::index_max);
		assert(false && "Max handle table size reached");
		return false;
	}

	size_t size = std::min(table.size() + chunk_size, HandleT::index_max);
	table.resize(size);
	return true;
}


template<typename HandleT, typename DataT, size_t chunk_size>
HandleT HandleMap<HandleT, DataT, chunk_size>::createHandle(DataT object) {

	size_t i = 0;

	// Find the next free space and create the handle
	for (; i < table.size(); ++i) {

		auto& h = table[i];

		if (h.second == nullptr) {
			if (h.first > HandleT::counter_max)
				h.first = 0;
			else
				h.first += 1;

			h.second = object;
			return HandleT{i, h.first};
		}
	}

	// If this point is reached, then the table needs to
	// be expanded before a new handle is created.
	if (allocateChunk()) {
		table[i].first = 1;
		table[i].second = object;
		return HandleT{i, table[i].first};
	}

	// Return an invalid handle if one couldn't be created
	return HandleT::invalid_handle;
}


template<typename HandleT, typename DataT, size_t chunk_size>
void HandleMap<HandleT, DataT, chunk_size>::releaseHandle(HandleT handle) {

	if (!isValid(handle)) {
		Logger::log(LogLevel::err, "Invalid handle specified for release (index: {}, counter: {})", handle.index, handle.counter);
		assert(false && "Invalid handle specified for release");
		return;
	}

	table[handle.index].second = nullptr;
}


template<typename HandleT, typename DataT, size_t chunk_size>
bool HandleMap<HandleT, DataT, chunk_size>::isValid(const HandleT& handle) const noexcept{

	if (handle != HandleT::invalid_handle && handle.index < table.size()) {
		const auto& entry = table[handle.index];
		return entry.first == handle.counter && entry.second != nullptr;
	}
	return false;
}