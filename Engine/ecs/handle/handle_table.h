#pragma once

#include "util\engine_util.h"
#include "ecs\handle\handle.h"


template<typename HandleT, typename DataT, size_t chunk_size = 512>
class HandleTable {
	private:
		using HandleEntry = std::pair<typename HandleT::value_type, DataT*>;

		// Potential expansion of HandleEntry
		/*
		struct HandleEntry {
			using ValueT = HandleT::value_type;
		
			HandleEntry() = default;
			explicit HandleEntry(ValueT next_index) : next_index(next_index) {}

			ValueT        next_index  : HandleT::n_index_bits;
			ValueT        counter     : HandleT::n_counter_bits;
			ValueT        end_of_list : 1;
			unique_ptr<T> entry;
		};
		*/


	public:
		HandleTable() { AllocateChunk(); }
		~HandleTable() = default;

		HandleT CreateHandle(DataT* object);
		void    ReleaseHandle(HandleT handle);

		bool Expired(const HandleT& handle) const {
			return handle.counter != handle_table[handle.index].first;
		}

		HandleT operator[](typename HandleT::value_type idx) const {
			assert(idx < handle_table.size() && "Invalid handle specified");
			return HandleT(idx, handles[idx].first);
		}

		DataT* operator[](HandleT handle) {
			auto pair = handle_table[handle.index];

			assert(handle.counter == pair.first && "Invalid handle specified");

			if (pair.first = handle.counter)
				return pair.second;

			return nullptr;
		}


	private:
		void AllocateChunk();


	private:
		vector<HandleEntry> handle_table;
};


#include "handle_table.tpp"