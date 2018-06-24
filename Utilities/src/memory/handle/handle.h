#pragma once

#include "datatypes/scalar_types.h"


// Handle template that can be specialized for any given data size,
// number of counter bits, and number of index bits

#pragma warning (push)
#pragma warning (disable: 4293) //disable '<<' undefined behavior warning

template<typename T, size_t CounterBits, size_t IndexBits>
struct Handle {

	// Ensure the Handle can hold the specified number of bits
	static_assert((CounterBits + IndexBits) <= (sizeof(T) * 8),
		"Size of handle type is smaller than number of bits specified");

public:
	Handle() {
	}

	explicit Handle(T value)
		: index(value & index_bitmask)
		, counter((value & (counter_bitmask << CounterBits)) >> CounterBits) {
	}

	explicit Handle(T index, T counter)
		: index(index)
		, counter(counter) {
	}


	operator T() const {
		return (counter << (sizeof(T) * 8 - CounterBits)) | index;
	}


public:
	T index : IndexBits;
	T counter : CounterBits;


private:
	static constexpr T index_bitmask = (1Ui64 << IndexBits) - 1Ui64;
	static constexpr T counter_bitmask = (1Ui64 << CounterBits) - 1Ui64;


public:
	using value_type = T;

	// Max values
	static constexpr T index_max = (1Ui64 << IndexBits) - 2Ui64;
	static constexpr T counter_max = (1Ui64 << CounterBits) - 2Ui64;

	// Number of bits
	static constexpr size_t n_counter_bits = CounterBits;
	static constexpr size_t n_index_bits = IndexBits;

	static constexpr T invalid_handle = std::numeric_limits<T>::max();
};

#pragma warning (pop)


using handle64 = Handle<u64, 24, 40>;
