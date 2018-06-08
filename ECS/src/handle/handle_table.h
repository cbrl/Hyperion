#pragma once

#include "handle/handle.h"
#include "log/log.h"


template<typename HandleT, typename DataT, size_t ChunkSize = 512>
class HandleTable {
private:
	using handle_entry = std::pair<typename HandleT::value_type, DataT*>;

	// Potential expansion of HandleEntry
	/*
	struct HandleEntry {
		using ValueT = HandleT::value_type;
	
		HandleEntry() = default;
		explicit HandleEntry(ValueT next_index) : next_index(next_index) {}

		ValueT next_index  : HandleT::n_index_bits;
		ValueT counter     : HandleT::n_counter_bits;
		ValueT end_of_list : 1;
		T*     entry;
	};
	*/


public:
	HandleTable() { allocateChunk(); }
	~HandleTable() = default;

	HandleT createHandle(DataT* object);
	void releaseHandle(HandleT handle);

	bool expired(const HandleT& handle) const {
		return handle.counter != handle_table[handle.index].first;
	}

	HandleT operator[](typename HandleT::value_type idx) const {
		assert(idx < handle_table.size() && "Invalid handle specified. Index out of range.");
		return HandleT(idx, handle_table[idx].first);
	}

	DataT* operator[](HandleT handle) {
		assert(handle.index != HandleT::invalid_handle && "Invalid handle specified");

		auto pair = handle_table[handle.index];

		assert(handle.counter == pair.first && "Invalid handle specified. Table counter and handle counter differ.");

		if (pair.first == handle.counter) {
			return pair.second;
		}
		return nullptr;
	}


private:
	void allocateChunk();


private:
	vector<handle_entry> handle_table;
};


#include "handle_table.tpp"
