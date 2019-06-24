#pragma once

#include "handle.h"
#include "datatypes/container_types.h"


template<typename HandleT, size_t chunk_size = 512>
class HandleTable {
	struct TableEntry {
		typename HandleT::value_type counter = 0;
		bool in_use = false;
	};

	using container_type = std::vector<TableEntry>;
	using handle_type    = HandleT;
	using size_type      = size_t;

public:

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	HandleTable() = default;
	HandleTable(const HandleTable&) = default;
	HandleTable(HandleTable&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~HandleTable() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	HandleTable& operator=(const HandleTable&) = default;
	HandleTable& operator=(HandleTable&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Modifiers
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	handle_type createHandle();

	void releaseHandle(handle_type handle);

	void clear() noexcept {
		table.clear();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Capacity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool valid(handle_type handle) const noexcept;

	[[nodiscard]]
	bool empty() const noexcept {
		return table.empty();
	}

	[[nodiscard]]
	size_type size() const noexcept {
		return table.size();
	}

	bool reserve(size_type new_cap) {
		if (table.capacity() < handle_type::index_max) {
			table.reserve(std::min(new_cap, handle_type::index_max));
			return true;
		}
		else {
			assert(false && "HandleTable::reserve() - max size reached");
			return false;
		}
	}

	bool resize(size_type new_size) {
		if (table.size() < handle_type::index_max) {
			table.resize(std::min(new_size, handle_type::index_max));
			return true;
		}
		else {
			assert(false && "HandleTable::resize() - max size reached");
			return false;
		}
	}

	void shrink_to_fit() {
		table.shrink_to_fit();
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	container_type table;
};

#include "handle_table.tpp"
