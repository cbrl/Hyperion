#pragma once

#include "datatypes/datatypes.h"

//----------------------------------------------------------------------------------
// Handle
//----------------------------------------------------------------------------------
// Handle template that can be specialized for any given data size,
// number of counter bits, and number of index bits
//----------------------------------------------------------------------------------

#pragma warning (push)
#pragma warning (disable: 4293) //disable '<<' undefined behavior warning

template<typename T, size_t IndexBits, size_t CounterBits>
struct Handle {

	//----------------------------------------------------------------------------------
	// Assertions
	//----------------------------------------------------------------------------------

	static_assert(std::is_integral_v<T>,
		"Handle template parameter is not an integral type");

	static_assert(CounterBits > 0 && CounterBits < sizeof(T) * 8,
		"Invalid counter bits specified for Handle");

	static_assert(IndexBits > 0 && IndexBits < sizeof(T) * 8,
		"Invalid index bits specified for Handle");

	static_assert((CounterBits + IndexBits) <= (sizeof(T) * 8),
		"Size of handle type is smaller than number of bits specified");

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr Handle() noexcept
		: Handle(invalid_handle) {
	}

	constexpr Handle(T value) noexcept
		: index(value & index_bitmask)
		, counter((value & counter_bitmask) >> CounterBits) {
	}

	constexpr Handle(T index, T counter) noexcept
		: index(index)
		, counter(counter) {
	}


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	constexpr operator T() const noexcept {
		return (counter << (sizeof(T) * 8 - CounterBits)) | index;
	}

	std::size_t hash() const noexcept {
		return std::hash<T>{}(this->operator T());
	}


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	T index   : IndexBits;
	T counter : CounterBits;


	//----------------------------------------------------------------------------------
	// Static Member Variables
	//----------------------------------------------------------------------------------

	// Type
	using value_type = T;

	// Max values
	static constexpr T index_max   = (T{1} << IndexBits) - T{2};
	static constexpr T counter_max = (T{1} << CounterBits) - T{2};

	// Invalid value
	static constexpr T invalid_handle = std::numeric_limits<T>::max();

	// Number of bits
	static constexpr size_t n_index_bits   = IndexBits;
	static constexpr size_t n_counter_bits = CounterBits;


private:
	// Bitmasks
	static constexpr T index_bitmask   = (T{1} << IndexBits) - T{1};
	static constexpr T counter_bitmask = ((T{1} << CounterBits) - T{1}) << CounterBits;
};

#pragma warning (pop)


//----------------------------------------------------------------------------------
// std::hash specialization
//----------------------------------------------------------------------------------
namespace std {
	template<typename T, size_t IB, size_t CB>
	struct hash<Handle<T,IB,CB>> {
		size_t operator()(const Handle<T, IB, CB>& handle) const noexcept {
			return handle.hash();
		}
	};
}


//----------------------------------------------------------------------------------
// using declarations
//----------------------------------------------------------------------------------
using handle64 = Handle<u64, 40, 24>;
static_assert(sizeof(handle64) == 8, "handle64 size is not 8 bytes");
static_assert(std::has_unique_object_representations_v<handle64>, "handle64 is not properly packed");
