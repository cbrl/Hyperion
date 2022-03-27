#pragma once

#include <array>
#include <tuple>
#include <type_traits>


namespace detail {
template <typename TupleT, size_t... indices>
[[nodiscard]]
constexpr std::array<std::common_type_t<std::tuple_element_t<indices, std::remove_reference_t<TupleT>>...>, sizeof...(indices)>
to_array(TupleT&& tuple, std::index_sequence<indices...>) {
	return {std::get<indices>(std::forward<TupleT>(tuple))...};
}
} // namespace detail

template <typename TupleT>
[[nodiscard]]
constexpr decltype(auto) to_array(TupleT&& tuple) {
	return detail::to_array(std::forward<TupleT>(tuple), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<TupleT>>>{});
}




//----------------------------------------------------------------------------------
// Vector
//----------------------------------------------------------------------------------
//
// A generic vector class that can be specialized for any type and dimension.
//
//----------------------------------------------------------------------------------
template<typename T, size_t N> requires (N >= 2)
class Vector : public std::array<T, N> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr Vector() noexcept : std::array<T, N>{0} {}

	constexpr Vector(const Vector&) noexcept = default;
	constexpr Vector(Vector&&) noexcept = default;
	
	constexpr Vector(T val) noexcept
	    : std::array<T, N>{} {
		this->fill(val); //constexpr since C++20
	}

	// Construct a Vector from N or less values. Members without a provided value are 0 initialized.
	template<typename... ArgsT>
	requires (std::convertible_to<ArgsT, T> && ...) && (sizeof...(ArgsT) <= N)
	constexpr Vector(ArgsT&&... args) noexcept
	    : std::array<T, N>{static_cast<T>(args)...} {
	}

// The following constructor causes the MSVC compiler to crash
#ifndef _MSC_VER
	// Construct a Vector from a number of other Vectors with a combined size <= N. Any members
	// without a provided value are 0 initialized.
	template<size_t... sizes> requires ((0 + ... + sizes) <= N)
	constexpr Vector(const Vector<T, sizes>&... arrays) noexcept
		: std::array<T, N>{ to_array(std::tuple_cat(static_cast<const std::array<T, sizes>&>(arrays)...,
		                                            std::array<T, N - (0 + ... + sizes)>{})) } {
	}
#endif
	
	// Construct a Vector from another Vector with size <= N and a number of values of type T. The
	// combined size of the other vector and the number of values must be <= N. Any members
	// without a provided value are 0 initialized.
	template<size_t other_N, typename... ArgsT>
	requires (std::convertible_to<ArgsT, T> && ... ) && (other_N + sizeof...(ArgsT) <= N)
	constexpr Vector(const Vector<T, other_N>& arr, ArgsT&&... args) noexcept
	    : std::array<T, N>{ to_array(std::tuple_cat(static_cast<const std::array<T, other_N>&>(arr),
		                                            std::make_tuple(static_cast<T>(args)...),
	                                                std::array<T, N - other_N - sizeof...(args)>{})) } {
	}
	

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Vector() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	constexpr Vector& operator=(const Vector&) noexcept = default;
	constexpr Vector& operator=(Vector&&) noexcept = default;
};



// Operator +=
template<typename T, typename U, size_t N, typename = decltype(std::declval<T>() + std::declval<U>())>
constexpr Vector<T, N>& operator+=(Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept {
	for (size_t i = 0; i < lhs.size(); ++i) {
		lhs[i] += rhs[i];
	}
	return lhs;
}
// Operator +=
template <typename T, typename U, size_t N, typename = decltype(std::declval<T>() + std::declval<U>())>
constexpr Vector<T, N>& operator+=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n += rhs;
	}
	return lhs;
}
// Operator +
template<typename T, typename U, size_t N, typename = decltype(std::declval<T>() + std::declval<U>())>
constexpr auto operator+(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept -> Vector<decltype(std::declval<T>() + std::declval<U>()), N> {
	Vector<decltype(std::declval<T>() + std::declval<U>()), N> out;
	std::copy(lhs.begin(), lhs.end(), out.begin());
	out += rhs;
	return out;
}
// Operator +
template <typename T, typename U, size_t N, typename = decltype(std::declval<T>() + std::declval<U>())>
constexpr auto operator+(const Vector<T, N>& lhs, U rhs) noexcept -> Vector<decltype(std::declval<T>() + std::declval<U>()), N> {
	Vector<decltype(std::declval<T>() + std::declval<U>()), N> out;
	std::copy(lhs.begin(), lhs.end(), out.begin());
	out += rhs;
	return out;
}


// Operator -=
template<typename T, typename U, size_t N, typename = decltype(std::declval<T>() - std::declval<U>())>
constexpr Vector<T, N>& operator-=(Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept {
	for (size_t i = 0; i < lhs.size(); ++i) {
		lhs[i] -= rhs[i];
	}
	return lhs;
}
// Operator -=
template <typename T, typename U, size_t N, typename = decltype(std::declval<T>() - std::declval<U>())>
constexpr Vector<T, N>& operator-=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n -= rhs;
	}
	return lhs;
}
// Operator -
template<typename T, typename U, size_t N, typename = decltype(std::declval<T>() - std::declval<U>())>
constexpr auto operator-(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept -> Vector<decltype(std::declval<T>() - std::declval<U>()), N> {
	Vector<decltype(std::declval<T>() + std::declval<U>()), N> out;
	std::copy(lhs.begin(), lhs.end(), out.begin());
	out -= rhs;
	return out;
}
// Operator -
template <typename T, typename U, size_t N, typename = decltype(std::declval<T>() - std::declval<U>())>
constexpr auto operator-(const Vector<T, N>& lhs, U rhs) noexcept -> Vector<decltype(std::declval<T>() - std::declval<U>()), N> {
	Vector<decltype(std::declval<T>() + std::declval<U>()), N> out;
	std::copy(lhs.begin(), lhs.end(), out.begin());
	out -= rhs;
	return out;
}



// Operator *=
template <typename T, typename U, size_t N, typename = decltype(std::declval<T>() * std::declval<U>())>
constexpr Vector<T, N>& operator*=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n *= rhs;
	}
	return lhs;
}
// Operator *
template<typename T, typename U, size_t N, typename = decltype(std::declval<T>() * std::declval<U>())>
constexpr auto operator*(const Vector<T, N>& lhs, U rhs) noexcept -> Vector<decltype(std::declval<T>() * std::declval<U>()), N> {
	Vector<decltype(std::declval<T>() + std::declval<U>()), N> out;
	std::copy(lhs.begin(), lhs.end(), out.begin());
	out *= rhs;
	return out;
}


// Operator /=
template<typename T, typename U, size_t N, typename = decltype(std::declval<T>() / std::declval<U>())>
constexpr Vector<T, N>& operator/=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n /= rhs;
	}
	return lhs;
}
// Operator /
template<typename T, typename U, size_t N, typename = decltype(std::declval<T>() / std::declval<U>())>
constexpr auto operator/(const Vector<T, N>& lhs, U rhs) noexcept -> Vector<decltype(std::declval<T>() / std::declval<U>()), N> {
	Vector<decltype(std::declval<T>() + std::declval<U>()), N> out;
	std::copy(lhs.begin(), lhs.end(), out.begin());
	out /= rhs;
	return out;
}



//----------------------------------------------------------------------------------
// i32 Vectors
//----------------------------------------------------------------------------------

using i32_2 = Vector<int32_t, 2>;
using i32_3 = Vector<int32_t, 3>;
using i32_4 = Vector<int32_t, 4>;

static_assert(sizeof(i32_2) == 8);
static_assert(sizeof(i32_3) == 12);
static_assert(sizeof(i32_4) == 16);

static_assert(std::is_standard_layout_v<i32_2>);
static_assert(std::is_standard_layout_v<i32_3>);
static_assert(std::is_standard_layout_v<i32_4>);

static_assert(std::is_trivially_copyable_v<i32_2>);
static_assert(std::is_trivially_copyable_v<i32_3>);
static_assert(std::is_trivially_copyable_v<i32_4>);



//----------------------------------------------------------------------------------
// i64 Vectors
//----------------------------------------------------------------------------------

using i64_2 = Vector<int64_t, 2>;
using i64_3 = Vector<int64_t, 3>;
using i64_4 = Vector<int64_t, 4>;

static_assert(sizeof(i64_2) == 16);
static_assert(sizeof(i64_3) == 24);
static_assert(sizeof(i64_4) == 32);

static_assert(std::is_standard_layout_v<i64_2>);
static_assert(std::is_standard_layout_v<i64_3>);
static_assert(std::is_standard_layout_v<i64_4>);

static_assert(std::is_trivially_copyable_v<i64_2>);
static_assert(std::is_trivially_copyable_v<i64_3>);
static_assert(std::is_trivially_copyable_v<i64_4>);



//----------------------------------------------------------------------------------
// u32 Vectors
//----------------------------------------------------------------------------------

using u32_2 = Vector<uint32_t, 2>;
using u32_3 = Vector<uint32_t, 3>;
using u32_4 = Vector<uint32_t, 4>;

static_assert(sizeof(u32_2) == 8);
static_assert(sizeof(u32_3) == 12);
static_assert(sizeof(u32_4) == 16);

static_assert(std::is_standard_layout_v<u32_2>);
static_assert(std::is_standard_layout_v<u32_3>);
static_assert(std::is_standard_layout_v<u32_4>);

static_assert(std::is_trivially_copyable_v<u32_2>);
static_assert(std::is_trivially_copyable_v<u32_3>);
static_assert(std::is_trivially_copyable_v<u32_4>);



//----------------------------------------------------------------------------------
// u64 Vectors
//----------------------------------------------------------------------------------

using u64_2 = Vector<uint64_t, 2>;
using u64_3 = Vector<uint64_t, 3>;
using u64_4 = Vector<uint64_t, 4>;

static_assert(sizeof(u64_2) == 16);
static_assert(sizeof(u64_3) == 24);
static_assert(sizeof(u64_4) == 32);

static_assert(std::is_standard_layout_v<u64_2>);
static_assert(std::is_standard_layout_v<u64_3>);
static_assert(std::is_standard_layout_v<u64_4>);

static_assert(std::is_trivially_copyable_v<u64_2>);
static_assert(std::is_trivially_copyable_v<u64_3>);
static_assert(std::is_trivially_copyable_v<u64_4>);



//----------------------------------------------------------------------------------
// f32 Vectors
//----------------------------------------------------------------------------------

using f32_2 = Vector<float, 2>;
using f32_3 = Vector<float, 3>;
using f32_4 = Vector<float, 4>;

static_assert(sizeof(f32_2) == 8);
static_assert(sizeof(f32_3) == 12);
static_assert(sizeof(f32_4) == 16);

static_assert(std::is_standard_layout_v<f32_2>);
static_assert(std::is_standard_layout_v<f32_3>);
static_assert(std::is_standard_layout_v<f32_4>);

static_assert(std::is_trivially_copyable_v<f32_2>);
static_assert(std::is_trivially_copyable_v<f32_3>);
static_assert(std::is_trivially_copyable_v<f32_4>);



//----------------------------------------------------------------------------------
// f64 Vectors
//----------------------------------------------------------------------------------

using f64_2 = Vector<double, 2>;
using f64_3 = Vector<double, 3>;
using f64_4 = Vector<double, 4>;

static_assert(sizeof(f64_2) == 16);
static_assert(sizeof(f64_3) == 24);
static_assert(sizeof(f64_4) == 32);

static_assert(std::is_standard_layout_v<f64_2>);
static_assert(std::is_standard_layout_v<f64_3>);
static_assert(std::is_standard_layout_v<f64_4>);

static_assert(std::is_trivially_copyable_v<f64_2>);
static_assert(std::is_trivially_copyable_v<f64_3>);
static_assert(std::is_trivially_copyable_v<f64_4>);
