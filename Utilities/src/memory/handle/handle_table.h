#pragma once

#include "handle.h"
#include "exception/exception.h"
#include "datatypes/container_types.h"


template<typename HandleT, typename DataT, size_t ChunkSize = 512>
class HandleTable {
private:
	using handle_entry = std::pair<typename HandleT::value_type, DataT*>;

	// Potential expansion of HandleEntry
	/*
	struct HandleEntry {
		using value_type = HandleT::value_type;
	
		HandleEntry() = default;
		explicit HandleEntry(ValueT next_index) : next_index(next_index) {}

		value_type next_index  : HandleT::n_index_bits;
		value_type counter     : HandleT::n_counter_bits;
		value_type end_of_list : 1;
		T*         entry;
	};
	*/


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	HandleTable() noexcept {
		allocateChunk();
	}

	HandleTable(const HandleTable& table) = delete;
	HandleTable(HandleTable&& table) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~HandleTable() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	HandleTable& operator=(const HandleTable& table) = delete;
	HandleTable& operator=(HandleTable&& table) noexcept = default;

	[[nodiscard]]
	DataT* operator[](HandleT handle) {
		bool valid = validHandle(handle);
		if (!valid) {
			Logger::log(LogLevel::err, "Invalid handle specified (index: {}, counter: {})", handle.index, handle.counter);
			assert(false && "HandleTable::operator[] - Invalid handle specified");
		}

		return valid ? table[handle.index].second : nullptr;
	}


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	HandleT createHandle(DataT* object);

	void releaseHandle(HandleT handle);

	bool validHandle(const HandleT& handle) const noexcept;


private:
	bool allocateChunk();


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::vector<handle_entry> table;
};


#include "handle_table.tpp"
