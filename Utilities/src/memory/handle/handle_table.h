#pragma once

#include "handle.h"


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
	HandleTable() { allocateChunk(); }
	~HandleTable() = default;

	[[nodiscard]]
	HandleT createHandle(DataT* object);

	void releaseHandle(HandleT handle);

	bool validHandle(const HandleT& handle) const;

	[[nodiscard]]
	DataT* operator[](HandleT handle) {
		assert(validHandle(handle) && "HandleTable::operator[](HandleT) - Invalid handle specified");

		auto [table_counter, data] = handle_table[handle.index];

		if (table_counter == handle.counter) {
			return data;
		}
		return nullptr;
	}


private:
	void allocateChunk();


private:
	vector<handle_entry> handle_table;
};


#include "handle_table.tpp"
