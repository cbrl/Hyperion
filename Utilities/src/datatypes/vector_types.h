#pragma once

#include <type_traits>


template<typename T>
struct Vector2 {

	static_assert(std::is_arithmetic_v<T>, "Vector2 template parameter is not an arithmetic type");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr Vector2() noexcept = default;
	constexpr Vector2(const Vector2<T>&) noexcept = default;
	constexpr Vector2(Vector2<T>&&) noexcept = default;

	constexpr Vector2(T n) noexcept
		: x(n)
		, y(n) {
	}

	constexpr Vector2(T x, T y) noexcept
		: x(x)
		, y(y){
	}

	~Vector2() noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the std::vector
	constexpr T* data() noexcept { return &this->x; }
	constexpr const T* data() const noexcept { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	// Operator =
	constexpr Vector2<T>& operator=(const Vector2<T>&) noexcept = default;
	constexpr Vector2<T>& operator=(Vector2<T>&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	T x{};
	T y{};
};


// Operator ==
template<typename T, typename U>
constexpr bool operator==(const Vector2<T>& lhs, const Vector2<U>& rhs) noexcept {
	return (lhs.x == rhs.x &&
	        lhs.y == rhs.y);
}
// Operator !=
template<typename T, typename U>
constexpr bool operator!=(const Vector2<T>& lhs, const Vector2<U>& rhs) noexcept {
	return !(lhs == rhs);
}


// Operator +=
template<typename T, typename U>
constexpr auto operator+=(Vector2<T>& lhs, const Vector2<U>& rhs) noexcept
	-> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>&> {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}
// Operator +
template<typename T, typename U>
constexpr auto operator+(Vector2<T> lhs, const Vector2<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>> {
	lhs += rhs;
	return lhs;
}


// Operator -=
template<typename T, typename U>
constexpr auto operator-=(Vector2<T>& lhs, const Vector2<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>&> {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}
// Operator -
template<typename T, typename U>
constexpr auto operator-(Vector2<T> lhs, const Vector2<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>> {
	lhs -= rhs;
	return lhs;
}


// Operator *=
template<typename T, typename U>
constexpr auto operator*=(Vector2<T>& lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>&> {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}
// Operator *
template<typename T, typename U>
constexpr auto operator*(Vector2<T> lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>> {
	lhs *= rhs;
	return lhs;
}


// Operator /=
template<typename T, typename U>
constexpr auto operator/=(Vector2<T>& lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>&> {
	lhs.x /= rhs;
	lhs.y /= rhs;
	return lhs;
}
// Operator /
template<typename T, typename U>
constexpr auto operator/(Vector2<T> lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector2<T>> {
	lhs /= rhs;
	return lhs;
}




template<typename T>
struct Vector3 {

	static_assert(std::is_arithmetic_v<T>, "Vector3 template parameter is not an arithmetic type");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr Vector3() noexcept = default;
	constexpr Vector3(const Vector3<T>&) noexcept = default;
	constexpr Vector3(Vector3<T>&&) noexcept = default;

	constexpr Vector3(T n) noexcept
		: x(n)
		, y(n)
		, z(n) {
	}

	constexpr Vector3(T x, T y, T z) noexcept
		: x(x)
		, y(y)
		, z(z) {
	}

	constexpr Vector3(const Vector2<T>& xy, T z) noexcept
		: x(xy.x)
		, y(xy.y)
		, z(z) {
	}

	~Vector3() noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the std::vector
	constexpr T* data() noexcept { return &this->x; }
	constexpr const T* data() const noexcept { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	constexpr Vector3<T>& operator=(const Vector3<T>&) noexcept = default;
	constexpr Vector3<T>& operator=(Vector3<T>&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	T x{};
	T y{};
	T z{};
};


// Operator ==
template<typename T, typename U>
constexpr bool operator==(const Vector3<T>& lhs, const Vector3<U>& rhs) noexcept {
	return (lhs.x == rhs.x &&
	        lhs.y == rhs.y &&
	        lhs.z == rhs.z);
}
// Operator !=
template<typename T, typename U>
constexpr bool operator!=(const Vector3<T>& lhs, const Vector3<U>& rhs) noexcept {
	return !(lhs == rhs);
}


// Operator +=
template<typename T, typename U>
constexpr auto operator+=(Vector3<T>& lhs, const Vector3<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>&> {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}
// Operator +
template<typename T, typename U>
constexpr auto operator+(Vector3<T> lhs, const Vector3<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>> {
	lhs += rhs;
	return lhs;
}


// Operator -=
template<typename T, typename U>
constexpr auto operator-=(Vector3<T>& lhs, const Vector3<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>&> {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}
// Operator -
template<typename T, typename U>
constexpr auto operator-(Vector3<T> lhs, const Vector3<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>> {
	lhs -= rhs;
	return lhs;
}


// Operator *=
template<typename T, typename U>
constexpr auto operator*=(Vector3<T>& lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>&> {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}
// Operator *
template<typename T, typename U>
constexpr auto operator*(Vector3<T> lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>> {
	lhs *= rhs;
	return lhs;
}


// Operator /=
template<typename T, typename U>
constexpr auto operator/=(Vector3<T>& lhs, U rhs) noexcept
	-> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>&> {
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	return lhs;
}
// Operator /
template<typename T, typename U>
constexpr auto operator/(Vector3<T> lhs, U rhs) noexcept
	-> std::enable_if_t<std::is_convertible_v<T, U>, Vector3<T>> {
	lhs /= rhs;
	return lhs;
}




template<typename T>
struct Vector4 {

	static_assert(std::is_arithmetic_v<T>, "Vector4 template parameter is not an arithmetic type");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr Vector4() noexcept = default;
	constexpr Vector4(const Vector4<T>&) noexcept = default;
	constexpr Vector4(Vector4<T>&&) noexcept = default;

	constexpr Vector4(T n) noexcept
		: x(n)
		, y(n)
		, z(n)
		, w(n) {
	}

	constexpr Vector4(T x, T y, T z, T w) noexcept
		: x(x)
		, y(y)
		, z(z)
		, w(w) {
	}

	constexpr Vector4(const Vector2<T>& xy, T z, T w) noexcept
		: x(xy.x)
		, y(xy.y)
		, z(z)
		, w(w) {
	}

	constexpr Vector4(const Vector3<T>& xyz, T w) noexcept
		: x(xyz.x)
		, y(xyz.y)
		, z(xyz.z)
		, w(w) {
	}

	~Vector4() noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the std::vector
	constexpr T* data() noexcept { return &this->x; }
	constexpr const T* data() const noexcept { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	constexpr Vector4<T>& operator=(const Vector4<T>&) noexcept = default;
	constexpr Vector4<T>& operator=(Vector4<T>&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	T x{};
	T y{};
	T z{};
	T w{};
};


// Operator ==
template<typename T, typename U>
constexpr bool operator==(const Vector4<T>& lhs, const Vector4<U>& rhs) noexcept{
	return (lhs.x == rhs.x &&
	        lhs.y == rhs.y &&
	        lhs.z == rhs.z &&
	        lhs.w == rhs.w);
}
// Operator !=
template<typename T, typename U>
constexpr bool operator!=(const Vector4<T>& lhs, const Vector4<U>& rhs) noexcept{
	return !(lhs == rhs);
}


// Operator +=
template<typename T, typename U>
constexpr auto operator+=(Vector4<T>& lhs, const Vector4<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>&> {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
	return lhs;
}
// Operator +
template<typename T, typename U>
constexpr auto operator+(Vector4<T> lhs, const Vector4<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>> {
	lhs += rhs;
	return lhs;
}


// Operator -=
template<typename T, typename U>
constexpr auto operator-=(Vector4<T>& lhs, const Vector4<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>&> {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;
	return lhs;
}
// Operator -
template<typename T, typename U>
constexpr auto operator-(Vector4<T> lhs, const Vector4<U>& rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>> {
	lhs -= rhs;
	return lhs;
}


// Operator *=
template<typename T, typename U>
constexpr auto operator*=(Vector4<T>& lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>&> {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	lhs.w *= rhs;
	return lhs;
}
// Operator *
template<typename T, typename U>
constexpr auto operator*(Vector4<T> lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>> {
	lhs *= rhs;
	return lhs;
}


// Operator /=
template<typename T, typename U>
constexpr auto operator/=(Vector4<T>& lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>&> {
	lhs.x /= rhs;
	lhs.y /= rhs;
	lhs.z /= rhs;
	lhs.w /= rhs;
	return lhs;
}
// Operator /
template<typename T, typename U>
constexpr auto operator/(Vector4<T> lhs, U rhs) noexcept
    -> std::enable_if_t<std::is_convertible_v<T, U>, Vector4<T>> {
	lhs /= rhs;
	return lhs;
}




//----------------------------------------------------------------------------------
// Signed Integer Vectors
//----------------------------------------------------------------------------------

using vec2_i32 = Vector2<int32_t>;
using vec3_i32 = Vector3<int32_t>;
using vec4_i32 = Vector4<int32_t>;

static_assert(sizeof(vec2_i32) == 8);
static_assert(sizeof(vec3_i32) == 12);
static_assert(sizeof(vec4_i32) == 16);


using vec2_i64 = Vector2<int64_t>;
using vec3_i64 = Vector3<int64_t>;
using vec4_i64 = Vector4<int64_t>;

static_assert(sizeof(vec2_i64) == 16);
static_assert(sizeof(vec3_i64) == 24);
static_assert(sizeof(vec4_i64) == 32);




//----------------------------------------------------------------------------------
// Unsigned Integer Vectors
//----------------------------------------------------------------------------------

using vec2_u32 = Vector2<uint32_t>;
using vec3_u32 = Vector3<uint32_t>;
using vec4_u32 = Vector4<uint32_t>;

static_assert(sizeof(vec2_u32) == 8);
static_assert(sizeof(vec3_u32) == 12);
static_assert(sizeof(vec4_u32) == 16);


using vec2_u64 = Vector2<uint64_t>;
using vec3_u64 = Vector3<uint64_t>;
using vec4_u64 = Vector4<uint64_t>;

static_assert(sizeof(vec2_u64) == 16);
static_assert(sizeof(vec3_u64) == 24);
static_assert(sizeof(vec4_u64) == 32);




//----------------------------------------------------------------------------------
// Floating Point Vectors
//----------------------------------------------------------------------------------

using vec2_f32 = Vector2<float>;
using vec3_f32 = Vector3<float>;
using vec4_f32 = Vector4<float>;

static_assert(sizeof(vec2_f32) == 8);
static_assert(sizeof(vec3_f32) == 12);
static_assert(sizeof(vec4_f32) == 16);


using vec2_f64 = Vector2<double>;
using vec3_f64 = Vector3<double>;
using vec4_f64 = Vector4<double>;

static_assert(sizeof(vec2_f64) == 16);
static_assert(sizeof(vec3_f64) == 24);
static_assert(sizeof(vec4_f64) == 32);
