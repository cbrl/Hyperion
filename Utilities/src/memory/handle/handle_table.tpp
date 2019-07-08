template<typename HandleT>
HandleT HandleTable<HandleT>::createHandle() {
	if (available > 0) {
		// Remove the first free handle from the list
		handle_type out{next, table[next].counter};
		next = table[next].index;
		--available;
		return out;
	}
	else {
		// Verify that the table size doesn't exceed the max handle index
		if (table.size() >= handle_type::index_max) {
			assert(false && "HandleTable::createHandle() - max size reached");
			return handle_type::invalid_handle();
		}

		// Insert and return a new handle
		const auto handle = table.emplace_back(table.size(), 0);
		return handle;
	}
}


template<typename HandleT>
void HandleTable<HandleT>::releaseHandle(handle_type handle) {
	if (not valid(handle)) {
		assert(false && "Invalid handle specified for release");
		return;
	}

	// Increment the handle version
	++table[handle.index].counter;

	// Extend list of free handles
	if (available > 0)
		table[handle.index].index = next;
	next = handle.index;
	++available;
}


template<typename HandleT>
bool HandleTable<HandleT>::valid(handle_type handle) const noexcept {
	if (handle != handle_type::invalid_handle() && handle.index < table.size()) {
		return table[handle.index].counter == handle.counter;
	}
	return false;
}