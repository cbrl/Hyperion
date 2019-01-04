#pragma once

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
template <typename T, size_t N, typename = std::enable_if_t<(N >= 2)>>
class Vector : public std::array<T, N> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	constexpr Vector() noexcept = default;
	constexpr Vector(const Vector&) noexcept = default;
	constexpr Vector(Vector&&) noexcept = default;
	
	constexpr Vector(T val) noexcept {
		this->fill(val); //constexpr since C++20
	}

	// Construct a Vector from N or less values. Members without a provided value are 0 initialized.
	template<typename... ArgsT,
	         typename = std::enable_if_t<std::conjunction_v<std::is_convertible<ArgsT, T>...>>,
	         typename = std::enable_if_t<sizeof...(ArgsT) <= N>>
	constexpr Vector(ArgsT&&... args) noexcept
	    : std::array<T, N>{static_cast<T>(args)...} {
	}

// The following constructor causes the MSVC compiler to crash
#ifndef _MSC_VER
	// Construct a Vector from a number of other Vectors with a combined size <= N. Any members
	// without a provided value are 0 initialized.
	template<size_t... sizes,
	         typename = std::enable_if_t<(0 + ... + sizes) <= N>>
	constexpr Vector(const Vector<T, sizes>&... arrays) noexcept
		: std::array<T, N>{ to_array(std::tuple_cat(static_cast<const std::array<T, sizes>&>(arrays)...,
		                                            std::array<T, N - (0 + ... + sizes)>{})) } {
	}
#endif
	
	// Construct a Vector from another Vector with size <= N and a number of values of type T. The
	// combined size of the other vector and the number of values must be <= N. Any members
	// without a provided value are 0 initialized.
	template<size_t other_N,
	         typename... ArgsT,
	         typename = std::enable_if_t<std::conjunction_v<std::is_convertible<ArgsT, T>...>>,
	         typename = std::enable_if_t<(other_N + sizeof...(ArgsT)) <= N>>
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



// Operator ==
template<typename T, size_t N>
constexpr bool operator==(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	return static_cast<const std::array<T, N>&>(lhs) == static_cast<const std::array<T, N>&>(rhs);
}
// Operator !=
template<typename T, size_t N>
constexpr bool operator!=(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	return static_cast<const std::array<T, N>&>(lhs) != static_cast<const std::array<T, N>&>(rhs);
}


// Operator <
template <typename T, size_t N>
constexpr bool operator<(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	return static_cast<const std::array<T, N>&>(lhs) < static_cast<const std::array<T, N>&>(rhs);
}
// Operator <=
template <typename T, size_t N>
constexpr bool operator<=(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	return static_cast<const std::array<T, N>&>(lhs) <= static_cast<const std::array<T, N>&>(rhs);
}


// Operator >
template <typename T, size_t N>
constexpr bool operator>(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	return static_cast<const std::array<T, N>&>(lhs) > static_cast<const std::array<T, N>&>(rhs);
}
// Operator >=
template <typename T, size_t N>
constexpr bool operator>=(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	return static_cast<const std::array<T, N>&>(lhs) >= static_cast<const std::array<T, N>&>(rhs);
}


// Operator +=
template<typename T, size_t N, typename = decltype(std::declval<T>() + std::declval<T>())>
constexpr Vector<T, N>& operator+=(Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	for (size_t i = 0; i < lhs.size(); ++i) {
		lhs[i] += rhs[i];
	}
	return lhs;
}
// Operator +=
template <typename T, size_t N, typename U, typename = decltype(std::declval<T>() + std::declval<U>())>
constexpr Vector<T, N>& operator+=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n += rhs;
	}
	return lhs;
}
// Operator +
template<typename T, size_t N, typename = decltype(std::declval<T>() + std::declval<T>())>
constexpr Vector<T, N> operator+(Vector<T, N> lhs, const Vector<T, N>& rhs) noexcept {
	lhs += rhs;
	return lhs;
}
// Operator +
template <typename T, size_t N, typename U, typename = decltype(std::declval<T>() + std::declval<U>())>
constexpr Vector<T, N> operator+(Vector<T, N> lhs, U rhs) noexcept {
	lhs += rhs;
	return lhs;
}


// Operator -=
template<typename T, size_t N, typename = decltype(std::declval<T>() - std::declval<T>())>
constexpr Vector<T, N>& operator-=(Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept {
	for (size_t i = 0; i < lhs.size(); ++i) {
		lhs[i] -= rhs[i];
	}
	return lhs;
}
// Operator -=
template <typename T, size_t N, typename U, typename = decltype(std::declval<T>() - std::declval<U>())>
constexpr Vector<T, N>& operator-=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n -= rhs;
	}
	return lhs;
}
// Operator -
template<typename T, size_t N, typename = decltype(std::declval<T>() - std::declval<T>())>
constexpr Vector<T, N> operator-(Vector<T, N> lhs, const Vector<T, N>& rhs) noexcept {
	lhs -= rhs;
	return lhs;
}
// Operator -
template <typename T, size_t N, typename U, typename = decltype(std::declval<T>() + std::declval<U>())>
constexpr Vector<T, N> operator-(Vector<T, N> lhs, U rhs) noexcept {
	lhs -= rhs;
	return lhs;
}



// Operator *=
template <typename T, size_t N, typename U, typename = decltype(std::declval<T>() - std::declval<U>())>
constexpr Vector<T, N>& operator*=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n *= rhs;
	}
	return lhs;
}
// Operator *
template<typename T, size_t N, typename U, typename = decltype(std::declval<T>() * std::declval<U>())>
constexpr Vector<T, N> operator*(Vector<T, N> lhs, U rhs) noexcept {
	lhs *= rhs;
	return lhs;
}


// Operator /=
template<typename T, size_t N, typename U, typename = decltype(std::declval<T>() / std::declval<U>())>
constexpr Vector<T, N>& operator/=(Vector<T, N>& lhs, U rhs) noexcept {
	for (auto& n : lhs) {
		n /= rhs;
	}
	return lhs;
}
// Operator /
template<typename T, size_t N, typename U, typename = decltype(std::declval<T>() / std::declval<U>())>
constexpr Vector<T, N> operator/(Vector<T, N> lhs, U rhs) noexcept {
	lhs /= rhs;
	return lhs;
}



//----------------------------------------------------------------------------------
// Signed Integer Vectors
//----------------------------------------------------------------------------------

using vec2_i32 = Vector<int32_t, 2>;
using vec3_i32 = Vector<int32_t, 3>;
using vec4_i32 = Vector<int32_t, 4>;

static_assert(sizeof(vec2_i32) == 8);
static_assert(sizeof(vec3_i32) == 12);
static_assert(sizeof(vec4_i32) == 16);

static_assert(std::is_pod_v<vec2_i32>);
static_assert(std::is_pod_v<vec3_i32>);
static_assert(std::is_pod_v<vec4_i32>);


using vec2_i64 = Vector<int64_t, 2>;
using vec3_i64 = Vector<int64_t, 3>;
using vec4_i64 = Vector<int64_t, 4>;

static_assert(sizeof(vec2_i64) == 16);
static_assert(sizeof(vec3_i64) == 24);
static_assert(sizeof(vec4_i64) == 32);

static_assert(std::is_pod_v<vec2_i64>);
static_assert(std::is_pod_v<vec3_i64>);
static_assert(std::is_pod_v<vec4_i64>);



//----------------------------------------------------------------------------------
// Unsigned Integer Vectors
//----------------------------------------------------------------------------------

using vec2_u32 = Vector<uint32_t, 2>;
using vec3_u32 = Vector<uint32_t, 3>;
using vec4_u32 = Vector<uint32_t, 4>;

static_assert(sizeof(vec2_u32) == 8);
static_assert(sizeof(vec3_u32) == 12);
static_assert(sizeof(vec4_u32) == 16);

static_assert(std::is_pod_v<vec2_u32>);
static_assert(std::is_pod_v<vec3_u32>);
static_assert(std::is_pod_v<vec4_u32>);


using vec2_u64 = Vector<uint64_t, 2>;
using vec3_u64 = Vector<uint64_t, 3>;
using vec4_u64 = Vector<uint64_t, 4>;

static_assert(sizeof(vec2_u64) == 16);
static_assert(sizeof(vec3_u64) == 24);
static_assert(sizeof(vec4_u64) == 32);

static_assert(std::is_pod_v<vec2_u64>);
static_assert(std::is_pod_v<vec3_u64>);
static_assert(std::is_pod_v<vec4_u64>);




//----------------------------------------------------------------------------------
// Floating Point Vectors
//----------------------------------------------------------------------------------

using vec2_f32 = Vector<float, 2>;
using vec3_f32 = Vector<float, 3>;
using vec4_f32 = Vector<float, 4>;

static_assert(sizeof(vec2_f32) == 8);
static_assert(sizeof(vec3_f32) == 12);
static_assert(sizeof(vec4_f32) == 16);

static_assert(std::is_pod_v<vec2_f32>);
static_assert(std::is_pod_v<vec3_f32>);
static_assert(std::is_pod_v<vec4_f32>);


using vec2_f64 = Vector<double, 2>;
using vec3_f64 = Vector<double, 3>;
using vec4_f64 = Vector<double, 4>;

static_assert(sizeof(vec2_f64) == 16);
static_assert(sizeof(vec3_f64) == 24);
static_assert(sizeof(vec4_f64) == 32);

static_assert(std::is_pod_v<vec2_f64>);
static_assert(std::is_pod_v<vec3_f64>);
static_assert(std::is_pod_v<vec4_f64>);
