#pragma once

#include "util\datatypes\datatypes.h"


// Handle template that can be specialized for any given data size,
// number of counter bits, and number of index bits

#pragma warning (push)
#pragma warning (disable: 4293) //disable '<<' undefined behavior warning

template<typename T, size_t counter_bits, size_t index_bits>
struct Handle {

	// Ensure the Handle can hold the specified number of bits
	static_assert((counter_bits + index_bits) <= (sizeof(T) * 8),
				  "Size of handle type is smaller than number of bits specified");

	public:
		Handle() : Handle(0, 0) {}

		Handle(T value)
			: index(value & index_bitmask)
			, counter((value & (counter_bitmask << counter_bits)) >> counter_bits)
		{}

		Handle(T index, T counter)
			: index(index)
			, counter(counter)
		{}


		operator T() const {
			return (counter << (sizeof(T) * 8 - counter_bits)) | index;
		}


	public:
		T index   : index_bits;
		T counter : counter_bits;


	private:
		static constexpr T index_bitmask = (1U << index_bits) - 1U;
		static constexpr T counter_bitmask = (1U << counter_bits) - 1U;


	public:
		using value_type = T;

		// Max values
		static constexpr T index_max   = (1U << index_bits) - 2U;
		static constexpr T counter_max = (1U << counter_bits) - 2U;

		// Number of bits
		static constexpr size_t n_counter_bits = counter_bits;
		static constexpr size_t n_index_bits   = index_bits;

		static constexpr T invalid_handle = std::numeric_limits<T>::max();
};

#pragma warning (pop)


using Handle64 = Handle<u64, 24, 40>;