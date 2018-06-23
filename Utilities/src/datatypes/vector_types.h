#pragma once

#include <type_traits>


template<typename T>
struct Vector2 {

	static_assert(std::is_arithmetic_v<T>, "Vec2 data type is not an arithmetic type");

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

	static_assert(std::is_arithmetic_v<T>, "Vec3 data type is not an arithmetic type");

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

	static_assert(std::is_arithmetic_v<T>, "Vec4 data type is not an arithetic type");

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

using i32_2 = Vector2<int32_t>;
using i32_3 = Vector3<int32_t>;
using i32_4 = Vector4<int32_t>;

static_assert(sizeof(i32_2) == 8);
static_assert(sizeof(i32_3) == 12);
static_assert(sizeof(i32_4) == 16);


using i64_2 = Vector2<int64_t>;
using i64_3 = Vector3<int64_t>;
using i64_4 = Vector4<int64_t>;

static_assert(sizeof(i64_2) == 16);
static_assert(sizeof(i64_3) == 24);
static_assert(sizeof(i64_4) == 32);




//----------------------------------------------------------------------------------
// Unsigned Integer Vectors
//----------------------------------------------------------------------------------

using u32_2 = Vector2<uint32_t>;
using u32_3 = Vector3<uint32_t>;
using u32_4 = Vector4<uint32_t>;

static_assert(sizeof(u32_2) == 8);
static_assert(sizeof(u32_3) == 12);
static_assert(sizeof(u32_4) == 16);


using u64_2 = Vector2<uint64_t>;
using u64_3 = Vector3<uint64_t>;
using u64_4 = Vector4<uint64_t>;

static_assert(sizeof(u64_2) == 16);
static_assert(sizeof(u64_3) == 24);
static_assert(sizeof(u64_4) == 32);




//----------------------------------------------------------------------------------
// Floating Point Vectors
//----------------------------------------------------------------------------------

using f32_2 = Vector2<float>;
using f32_3 = Vector3<float>;
using f32_4 = Vector4<float>;

static_assert(sizeof(f32_2) == 8);
static_assert(sizeof(f32_3) == 12);
static_assert(sizeof(f32_4) == 16);


using f64_2 = Vector2<double>;
using f64_3 = Vector3<double>;
using f64_4 = Vector4<double>;

static_assert(sizeof(f64_2) == 16);
static_assert(sizeof(f64_3) == 24);
static_assert(sizeof(f64_4) == 32);
