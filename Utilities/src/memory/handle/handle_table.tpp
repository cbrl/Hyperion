template<typename HandleT, size_t chunk_size>
HandleT HandleTable<HandleT, chunk_size>::createHandle() {
	size_t i = 0;

	// Find the next free space and create the handle
	for (; i < table.size(); ++i) {
		auto& entry = table[i];

		if (entry.in_use == false) {
			if (entry.counter > HandleT::counter_max)
				entry.counter = 0;
			else
				entry.counter += 1;

			entry.in_use = true;
			return HandleT{i, entry.counter};
		}
	}

	// If this point is reached, then the table needs to
	// be expanded before a new handle is created.
	if (resize(table.capacity() + chunk_size)) {
		table[i].counter = 1;
		table[i].in_use = true;
		return HandleT{i, table[i].counter};
	}

	// Return an invalid handle if one couldn't be created
	return HandleT{};
}


template<typename HandleT, size_t chunk_size>
void HandleTable<HandleT, chunk_size>::releaseHandle(handle_type handle) {
	if (!valid(handle)) {
		assert(false && "Invalid handle specified for release");
		return;
	}

	table[handle.index].in_use = false;
}


template<typename HandleT, size_t chunk_size>
bool HandleTable<HandleT, chunk_size>::valid(handle_type handle) const noexcept {

	if (handle != HandleT::invalid_handle && handle.index < table.size()) {
		const auto& entry = table[handle.index];
		return (entry.counter == handle.counter) && (entry.in_use == true);
	}
	return false;
}