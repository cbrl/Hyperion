#pragma once

#include "datatypes/scalar_types.h"
#include <type_traits>
#include <limits>
#include <concepts>

//----------------------------------------------------------------------------------
// Handle
//----------------------------------------------------------------------------------
// Handle template that can be specialized for any given data size,
// number of counter bits, and number of index bits
//----------------------------------------------------------------------------------

template<std::unsigned_integral T, size_t IndexBits, size_t CounterBits>
struct Handle {
public:
	using value_type = T;


	//----------------------------------------------------------------------------------
	// Assertions
	//----------------------------------------------------------------------------------
	static_assert((CounterBits > 0) && (CounterBits < (sizeof(T) * 8)),
		"Invalid number of counter bits specified for Handle");

	static_assert((IndexBits > 0) && (IndexBits < (sizeof(T) * 8)),
		"Invalid number of index bits specified for Handle");

	static_assert((CounterBits + IndexBits) <= (sizeof(T) * 8),
		"Size of handle type is smaller than specified IndexBits + CounterBits");


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr Handle() noexcept = default;

	constexpr explicit Handle(T value) noexcept
		: index((value & index_bitmask) >> counter_bits)
		, counter(value & counter_bitmask) {
	}

	constexpr explicit Handle(T index, T counter) noexcept
		: index(index)
		, counter(counter) {
	}

	constexpr Handle(const Handle&) noexcept = default;
	constexpr Handle(Handle&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Handle() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr Handle& operator=(const Handle&) noexcept = default;
	constexpr Handle& operator=(Handle&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	constexpr operator T() const noexcept {
		return (index << counter_bits) | counter;
	}

	[[nodiscard]]
	std::size_t hash() const noexcept {
		return std::hash<T>{}(this->operator T());
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	T index   : IndexBits   = std::numeric_limits<T>::max();
	T counter : CounterBits = std::numeric_limits<T>::max();


	//----------------------------------------------------------------------------------
	// Static Members
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	static constexpr Handle invalid_handle() noexcept {
		return Handle{std::numeric_limits<T>::max()};
	}

	// Max values
	static constexpr T index_max   = (T{1} << IndexBits) - T{2};
	static constexpr T counter_max = (T{1} << CounterBits) - T{2};

	// Number of bits
	static constexpr size_t index_bits   = IndexBits;
	static constexpr size_t counter_bits = CounterBits;

private:

	// Bitmasks
	static constexpr T index_bitmask   = ((T{1} << IndexBits) - T{1}) << CounterBits;
	static constexpr T counter_bitmask = (T{1} << CounterBits) - T{1};
};


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
using handle32 = Handle<u32, 20, 12>;
static_assert(sizeof(handle32) == 4, "handle32 size is not 4 bytes");
static_assert(std::has_unique_object_representations_v<handle32>, "handle32 is not properly packed");

using handle64 = Handle<u64, 40, 24>;
static_assert(sizeof(handle64) == 8, "handle64 size is not 8 bytes");
static_assert(std::has_unique_object_representations_v<handle64>, "handle64 is not properly packed");
