#pragma once

#include "handle.h"
#include "datatypes/container_types.h"
#include <assert.h>


//----------------------------------------------------------------------------------
// HandleTable
//----------------------------------------------------------------------------------
//
// The handle table creates, stores, and manages handles.
// The table can be queried for the validity of a handle it has created.
//
//----------------------------------------------------------------------------------

template<typename HandleT>
class HandleTable {
	using container_type = std::vector<HandleT>;

public:

	using handle_type    = HandleT;
	using size_type      = size_t;

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
		available = 0;
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

	void reserve(size_type new_cap) {
		if (table.capacity() < handle_type::index_max) {
			table.reserve(std::min(new_cap, handle_type::index_max));
		}
		else {
			assert(false && "HandleTable::reserve() - max size reached");
			return;
		}
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	container_type table;

	typename handle_type::value_type next = 0; //index of next free handle
	typename handle_type::value_type available = 0; //number of free handles
};

#include "handle_table.tpp"
