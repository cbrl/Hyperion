#pragma once

#include <type_traits>


template<typename T>
struct Vector2 {

	static_assert(std::is_arithmetic_v<T>, "Vector2 template parameter is not an arithmetic type");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr Vector2() noexcept = default;
	constexpr Vector2(const Vector2<T>& vec) noexcept = default;
	constexpr Vector2(Vector2<T>&& vec) noexcept = default;

	constexpr Vector2(T _x, T _y) noexcept
		: x(_x)
		, y(_y){
	}

	~Vector2() noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the vector
	constexpr T* data() { return &this->x; }
	constexpr const T* data() const { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	constexpr Vector2<T>& operator=(const Vector2<T>& copy) noexcept = default;
	constexpr Vector2<T>& operator=(Vector2<T>&& move) noexcept = default;


	// Operator == and !=
	constexpr bool operator==(const Vector2<T>& compare) noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y);
	}

	constexpr bool operator==(const Vector2<T>& compare) const noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y);
	}

	constexpr bool operator!=(const Vector2<T>& compare) noexcept {
		return !(*this == compare);
	}

	constexpr bool operator!=(const Vector2<T>& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	constexpr Vector2<T>& operator+=(const Vector2<T>& in) noexcept {
		this->x += in.x;
		this->y += in.y;
		return *this;
	}

	constexpr Vector2<T> operator+(const Vector2<T>& in) noexcept {
		Vector2<T> v(*this);
		v += in;
		return v;
	}

	constexpr Vector2<T> operator+(const Vector2<T>& in) const noexcept {
		Vector2<T> v(*this);
		v += in;
		return v;
	}


	// Operator -
	constexpr Vector2<T>& operator-=(const Vector2<T>& in) noexcept {
		this->x -= in.x;
		this->y -= in.y;
		return *this;
	}

	constexpr Vector2<T> operator-(const Vector2<T>& in) noexcept {
		Vector2<T> v(*this);
		v -= in;
		return v;
	}

	constexpr Vector2<T> operator-(const Vector2<T>& in) const noexcept {
		Vector2<T> v(*this);
		v -= in;
		return v;
	}


	// Operator *
	constexpr Vector2<T>& operator*=(T in) noexcept {
		this->x *= in;
		this->y *= in;
		return *this;
	}

	constexpr Vector2<T> operator*(T in) noexcept {
		Vector2<T> v(*this);
		v *= in;
		return v;
	}

	constexpr Vector2<T> operator*(T in) const noexcept {
		Vector2<T> v(*this);
		v *= in;
		return v;
	}


	// Operator /
	constexpr Vector2<T>& operator/=(T in) noexcept {
		this->x /= in;
		this->y /= in;
		return *this;
	}

	constexpr Vector2<T> operator/(T in) noexcept {
		Vector2<T> v(*this);
		v /= in;
		return v;
	}

	constexpr Vector2<T> operator/(T in) const noexcept {
		Vector2<T> v(*this);
		v /= in;
		return v;
	}

	#pragma endregion operators


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	T x{};
	T y{};
};




template<typename T>
struct Vector3 {

	static_assert(std::is_arithmetic_v<T>, "Vector3 template parameter is not an arithmetic type");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr Vector3() noexcept = default;
	constexpr Vector3(const Vector3<T>& vec) noexcept = default;
	constexpr Vector3(Vector3<T>&& vec) noexcept = default;

	constexpr Vector3(T _x, T _y, T _z) noexcept
		: x(_x)
		, y(_y)
		, z(_z) {
	}

	constexpr Vector3(const Vector2<T>& _xy, T _z) noexcept
		: x(_xy.x)
		, y(_xy.y)
		, z(_z) {
	}

	~Vector3() noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the vector
	constexpr T* data() { return &this->x; }
	constexpr const T* data() const { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	constexpr Vector3<T>& operator=(const Vector3<T>& copy) noexcept = default;
	constexpr Vector3<T>& operator=(Vector3<T>&& move) noexcept = default;


	// Operator == and !=
	constexpr bool operator==(const Vector3<T>& compare) noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z);
	}

	constexpr bool operator==(const Vector3<T>& compare) const noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z);
	}

	constexpr bool operator!=(const Vector3<T>& compare) noexcept {
		return !(*this == compare);
	}

	constexpr bool operator!=(const Vector3<T>& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	constexpr Vector3<T>& operator+=(const Vector3<T>& in) noexcept {
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;
		return *this;
	}

	constexpr Vector3<T> operator+(const Vector3<T>& in) noexcept {
		Vector3<T> v(*this);
		v += in;
		return v;
	}

	constexpr Vector3<T> operator+(const Vector3<T>& in) const noexcept {
		Vector3<T> v(*this);
		v += in;
		return v;
	}


	// Operator -
	constexpr Vector3<T>& operator-=(const Vector3<T>& in) noexcept {
		this->x -= in.x;
		this->y -= in.y;
		this->z -= in.z;
		return *this;
	}

	constexpr Vector3<T> operator-(const Vector3<T>& in) noexcept {
		Vector3<T> v(*this);
		v -= in;
		return v;
	}

	constexpr Vector3<T> operator-(const Vector3<T>& in) const noexcept {
		Vector3<T> v(*this);
		v -= in;
		return v;
	}


	// Operator *
	constexpr Vector3<T>& operator*=(T in) noexcept {
		this->x *= in;
		this->y *= in;
		this->z *= in;
		return *this;
	}

	constexpr Vector3<T> operator*(T in) noexcept {
		Vector3<T> v(*this);
		v *= in;
		return v;
	}

	constexpr Vector3<T> operator*(T in) const noexcept {
		Vector3<T> v(*this);
		v *= in;
		return v;
	}


	// Operator /
	constexpr Vector3<T>& operator/=(T in) noexcept {
		this->x /= in;
		this->y /= in;
		this->z /= in;
		return *this;
	}

	constexpr Vector3<T> operator/(T in) noexcept {
		Vector3<T> v(*this);
		v /= in;
		return v;
	}

	constexpr Vector3<T> operator/(T in) const noexcept {
		Vector3<T> v(*this);
		v /= in;
		return v;
	}

	#pragma endregion operators


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	T x{};
	T y{};
	T z{};
};




template<typename T>
struct Vector4 {

	static_assert(std::is_arithmetic_v<T>, "Vector4 template parameter is not an arithetic type");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	constexpr Vector4() noexcept = default;
	constexpr Vector4(const Vector4<T>& _xyzw) noexcept = default;
	constexpr Vector4(Vector4<T>&& vec) noexcept = default;

	constexpr Vector4(T _x, T _y, T _z, T _w) noexcept
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w) {
	}

	constexpr Vector4(const Vector2<T>& _xy, T _z, T _w) noexcept
		: x(_xy.x)
		, y(_xy.y)
		, z(_z)
		, w(_w) {
	}

	constexpr Vector4(const Vector3<T>& _xyz, T _w) noexcept
		: x(_xyz.x)
		, y(_xyz.y)
		, z(_xyz.z)
		, w(_w) {
	}

	~Vector4() noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the vector
	constexpr T* data() noexcept { return &this->x; }
	constexpr const T* data() const noexcept { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	constexpr Vector4<T>& operator=(const Vector4<T>& copy) noexcept = default;
	constexpr Vector4<T>& operator=(Vector4<T>&& move) noexcept = default;


	// Operator == and !=
	constexpr bool operator==(const Vector4<T>& compare) noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z &&
		        this->w == compare.w);
	}

	constexpr bool operator==(const Vector4<T>& compare) const noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z &&
		        this->w == compare.w);
	}

	constexpr bool operator!=(const Vector4<T>& compare) noexcept {
		return !(*this == compare);
	}

	constexpr bool operator!=(const Vector4<T>& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	constexpr Vector4<T>& operator+=(const Vector4<T>& in) noexcept {
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;
		this->w += in.w;
		return *this;
	}

	constexpr Vector4<T> operator+(const Vector4<T>& in) noexcept {
		Vector4<T> v(*this);
		v += in;
		return v;
	}

	constexpr Vector4<T> operator+(const Vector4<T>& in) const noexcept {
		Vector4<T> v(*this);
		v += in;
		return v;
	}


	// Operator -
	constexpr Vector4<T>& operator-=(const Vector4<T>& in) noexcept {
		this->x -= in.x;
		this->y -= in.y;
		this->z -= in.z;
		this->w -= in.w;
		return *this;
	}

	constexpr Vector4<T> operator-(const Vector4<T>& in) noexcept {
		Vector4<T> v(*this);
		v -= in;
		return v;
	}

	constexpr Vector4<T> operator-(const Vector4<T>& in) const noexcept {
		Vector4<T> v(*this);
		v -= in;
		return v;
	}


	// Operator *
	constexpr Vector4<T>& operator*=(T in) noexcept {
		this->x *= in;
		this->y *= in;
		this->z *= in;
		this->w *= in;
		return *this;
	}

	constexpr Vector4<T> operator*(T in) noexcept {
		Vector4<T> v(*this);
		v *= in;
		return v;
	}

	constexpr Vector4<T> operator*(T in) const noexcept {
		Vector4<T> v(*this);
		v *= in;
		return v;
	}


	// Operator /
	constexpr Vector4<T>& operator/=(T in) noexcept {
		this->x /= in;
		this->y /= in;
		this->z /= in;
		this->w /= in;
		return *this;
	}

	constexpr Vector4<T> operator/(T in) noexcept {
		Vector4<T> v(*this);
		v /= in;
		return v;
	}

	constexpr Vector4<T> operator/(T in) const noexcept {
		Vector4<T> v(*this);
		v /= in;
		return v;
	}

	#pragma endregion operators


	//----------------------------------------------------------------------------------
	// Members
	//----------------------------------------------------------------------------------
	T x{};
	T y{};
	T z{};
	T w{};
};




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
