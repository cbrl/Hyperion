#pragma once


//----------------------------------------------------------------------------------
// Default base vectors
//----------------------------------------------------------------------------------
template<typename T>
struct BaseVec2 {
	BaseVec2() noexcept = default;

	BaseVec2(T _x, T _y) noexcept
		: x(_x)
		, y(_y) {
	}

	T x;
	T y;
};


template<typename T>
struct BaseVec3 {
	BaseVec3() noexcept = default;

	BaseVec3(T _x, T _y, T _z) noexcept
		: x(_x)
		, y(_y)
		, z(_z) {
	}

	T x;
	T y;
	T z;
};


template<typename T>
struct BaseVec4 {
	BaseVec4() noexcept = default;

	BaseVec4(T _x, T _y, T _z, T _w) noexcept
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w) {
	}

	T x;
	T y;
	T z;
	T w;
};


//----------------------------------------------------------------------------------
// Vec2
//----------------------------------------------------------------------------------
template<typename DataT, typename DerivedT = BaseVec2<DataT>>
struct Vec2 : public DerivedT {

	static_assert(std::is_arithmetic_v<DataT>, "Vec2 data type is not an arithmetic type");
	static_assert((sizeof(DerivedT) / sizeof(DataT)) == 2, "Vec2 base class does not have 2 members");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Vec2() noexcept = default;
	Vec2(Vec2<DataT, DerivedT>&& vec) noexcept = default;
	~Vec2() noexcept = default;

	Vec2(DataT _x, DataT _y) noexcept : DerivedT(_x, _y) {
	}

	Vec2(const Vec2<DataT, DerivedT>& _xy) noexcept : DerivedT(_xy) {
	}


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the vector
	constexpr DataT* data() { return &this->x; }
	constexpr const DataT* data() const { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	Vec2<DataT, DerivedT>& operator=(const Vec2<DataT, DerivedT>& copy) noexcept {
		this->x = copy.x;
		this->y = copy.y;
		return *this;
	}

	Vec2<DataT, DerivedT>& operator=(Vec2<DataT, DerivedT>&& move) noexcept {
		this->x = std::move(move.x);
		this->y = std::move(move.y);
		return *this;
	}

	// Operator == and !=
	constexpr bool operator==(const Vec2<DataT, DerivedT>& compare) noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y);
	}

	constexpr bool operator==(const Vec2<DataT, DerivedT>& compare) const noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y);
	}

	constexpr bool operator!=(const Vec2<DataT, DerivedT>& compare) noexcept {
		return !(*this == compare);
	}

	constexpr bool operator!=(const Vec2<DataT, DerivedT>& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	Vec2<DataT, DerivedT>& operator+=(const Vec2<DataT, DerivedT>& in) noexcept {
		this->x += in.x;
		this->y += in.y;
		return *this;
	}

	Vec2<DataT, DerivedT> operator+(const Vec2<DataT, DerivedT>& in) noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v += in;
		return v;
	}

	Vec2<DataT, DerivedT> operator+(const Vec2<DataT, DerivedT>& in) const noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v += in;
		return v;
	}


	// Operator -
	Vec2<DataT, DerivedT>& operator-=(const Vec2<DataT, DerivedT>& in) noexcept {
		this->x -= in.x;
		this->y -= in.y;
		return *this;
	}

	Vec2<DataT, DerivedT> operator-(const Vec2<DataT, DerivedT>& in) noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v -= in;
		return v;
	}

	Vec2<DataT, DerivedT> operator-(const Vec2<DataT, DerivedT>& in) const noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v -= in;
		return v;
	}


	// Operator *
	Vec2<DataT, DerivedT>& operator*=(DataT in) noexcept {
		this->x *= in;
		this->y *= in;
		return *this;
	}

	Vec2<DataT, DerivedT> operator*(DataT in) noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v *= in;
		return v;
	}

	Vec2<DataT, DerivedT> operator*(DataT in) const noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v *= in;
		return v;
	}


	// Operator /
	Vec2<DataT, DerivedT>& operator/=(DataT in) noexcept {
		this->x /= in;
		this->y /= in;
		return *this;
	}

	Vec2<DataT, DerivedT> operator/(DataT in) noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v /= in;
		return v;
	}

	Vec2<DataT, DerivedT> operator/(DataT in) const noexcept {
		Vec2<DataT, DerivedT> v(*this);
		v /= in;
		return v;
	}

	#pragma endregion operators
};


//----------------------------------------------------------------------------------
// Vec3
//----------------------------------------------------------------------------------
template<typename DataT, typename DerivedT = BaseVec3<DataT>>
struct Vec3 : public DerivedT {

	static_assert(std::is_arithmetic_v<DataT>, "Vec3 data type is not an arithmetic type");
	static_assert((sizeof(DerivedT) / sizeof(DataT)) == 3, "Vec3 base class does not have 3 members");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Vec3() noexcept = default;
	Vec3(Vec3<DataT, DerivedT>&& vec) noexcept = default;
	~Vec3() noexcept = default;

	Vec3(DataT _x, DataT _y, DataT _z) noexcept : DerivedT(_x, _y, _z) {
	}

	Vec3(const Vec2<DataT, DerivedT>& _xy, DataT _z) noexcept : DerivedT(_xy.x, _xy.y, _z) {
	}

	Vec3(const Vec3<DataT, DerivedT>& _xyz) noexcept : DerivedT(_xyz.x, _xyz.y, _xyz.z) {
	}


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the vector
	constexpr DataT* data() { return &this->x; }
	constexpr const DataT* data() const { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	Vec3<DataT, DerivedT>& operator=(const Vec3<DataT, DerivedT>& copy) noexcept {
		this->x = copy.x;
		this->y = copy.y;
		this->z = copy.z;
		return *this;
	}

	Vec3<DataT, DerivedT>& operator=(Vec3<DataT, DerivedT>&& move) noexcept {
		this->x = std::move(move.x);
		this->y = std::move(move.y);
		this->z = std::move(move.z);
		return *this;
	}

	// Operator == and !=
	constexpr bool operator==(const Vec3<DataT, DerivedT>& compare) noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z);
	}

	constexpr bool operator==(const Vec3<DataT, DerivedT>& compare) const noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z);
	}

	constexpr bool operator!=(const Vec3<DataT, DerivedT>& compare) noexcept {
		return !(*this == compare);
	}

	constexpr bool operator!=(const Vec3<DataT, DerivedT>& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	Vec3<DataT, DerivedT>& operator+=(const Vec3<DataT, DerivedT>& in) noexcept {
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;
		return *this;
	}

	Vec3<DataT, DerivedT> operator+(const Vec3<DataT, DerivedT>& in) noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v += in;
		return v;
	}

	Vec3<DataT, DerivedT> operator+(const Vec3<DataT, DerivedT>& in) const noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v += in;
		return v;
	}


	// Operator -
	Vec3<DataT, DerivedT>& operator-=(const Vec3<DataT, DerivedT>& in) noexcept {
		this->x -= in.x;
		this->y -= in.y;
		this->z -= in.z;
		return *this;
	}

	Vec3<DataT, DerivedT> operator-(const Vec3<DataT, DerivedT>& in) noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v -= in;
		return v;
	}

	Vec3<DataT, DerivedT> operator-(const Vec3<DataT, DerivedT>& in) const noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v -= in;
		return v;
	}


	// Operator *
	Vec3<DataT, DerivedT>& operator*=(DataT in) noexcept {
		this->x *= in;
		this->y *= in;
		this->z *= in;
		return *this;
	}

	Vec3<DataT, DerivedT> operator*(DataT in) noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v *= in;
		return v;
	}

	Vec3<DataT, DerivedT> operator*(DataT in) const noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v *= in;
		return v;
	}


	// Operator /
	Vec3<DataT, DerivedT>& operator/=(DataT in) noexcept {
		this->x /= in;
		this->y /= in;
		this->z /= in;
		return *this;
	}

	Vec3<DataT, DerivedT> operator/(DataT in) noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v /= in;
		return v;
	}

	Vec3<DataT, DerivedT> operator/(DataT in) const noexcept {
		Vec3<DataT, DerivedT> v(*this);
		v /= in;
		return v;
	}

	#pragma endregion operators
};


//----------------------------------------------------------------------------------
// Vec4
//----------------------------------------------------------------------------------
template<typename DataT, typename DerivedT = BaseVec4<DataT>>
struct Vec4 : public DerivedT {

	static_assert(std::is_arithmetic_v<DataT>, "Vec4 data type is not an arithetic type");
	static_assert((sizeof(DerivedT) / sizeof(DataT)) == 4, "Vec4 base class does not have 4 members");

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Vec4() noexcept = default;
	Vec4(Vec4&& vec) noexcept = default;
	~Vec4() noexcept = default;

	Vec4(DataT _x, DataT _y, DataT _z, DataT _w) noexcept : DerivedT(_x, _y, _z, _w) {
	}

	Vec4(const Vec2<DataT, DerivedT>& _xy, DataT _z, DataT _w) noexcept : DerivedT(_xy.x, _xy.y, _z, _w) {
	}

	Vec4(const Vec3<DataT, DerivedT>& _xyz, DataT _w) noexcept : DerivedT(_xyz.x, _xyz.y, _xyz.z, _w) {
	}

	Vec4(const Vec4<DataT, DerivedT>& _xyzw) noexcept : DerivedT(_xyzw.x, _xyzw.y, _xyzw.z, _xyzw.w) {
	}


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the vector
	constexpr DataT* data() noexcept { return &this->x; }
	constexpr const DataT* data() const noexcept { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator =
	Vec4<DataT, DerivedT>& operator=(const Vec4<DataT, DerivedT>& copy) noexcept {
		this->x = copy.x;
		this->y = copy.y;
		this->z = copy.z;
		this->w = copy.w;
		return *this;
	}

	Vec4<DataT, DerivedT>& operator=(Vec4<DataT, DerivedT>&& move) noexcept {
		this->x = std::move(move.x);
		this->y = std::move(move.y);
		this->z = std::move(move.z);
		this->w = std::move(move.w);
		return *this;
	}

	// Operator == and !=
	constexpr bool operator==(const Vec4<DataT, DerivedT>& compare) noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z &&
		        this->w == compare.w);
	}

	constexpr bool operator==(const Vec4<DataT, DerivedT>& compare) const noexcept {
		return (this->x == compare.x &&
		        this->y == compare.y &&
		        this->z == compare.z &&
		        this->w == compare.w);
	}

	constexpr bool operator!=(const Vec4<DataT, DerivedT>& compare) noexcept {
		return !(*this == compare);
	}

	constexpr bool operator!=(const Vec4<DataT, DerivedT>& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	Vec4<DataT, DerivedT>& operator+=(const Vec4<DataT, DerivedT>& in) noexcept {
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;
		this->w += in.w;
		return *this;
	}

	Vec4<DataT, DerivedT> operator+(const Vec4<DataT, DerivedT>& in) noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v += in;
		return v;
	}

	Vec4<DataT, DerivedT> operator+(const Vec4<DataT, DerivedT>& in) const noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v += in;
		return v;
	}


	// Operator -
	Vec4<DataT, DerivedT>& operator-=(const Vec4<DataT, DerivedT>& in) noexcept {
		this->x -= in.x;
		this->y -= in.y;
		this->z -= in.z;
		this->w -= in.w;
		return *this;
	}

	Vec4<DataT, DerivedT> operator-(const Vec4<DataT, DerivedT>& in) noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v -= in;
		return v;
	}

	Vec4<DataT, DerivedT> operator-(const Vec4<DataT, DerivedT>& in) const noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v -= in;
		return v;
	}


	// Operator *
	Vec4<DataT, DerivedT>& operator*=(DataT in) noexcept {
		this->x *= in;
		this->y *= in;
		this->z *= in;
		this->w *= in;
		return *this;
	}

	Vec4<DataT, DerivedT> operator*(DataT in) noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v *= in;
		return v;
	}

	Vec4<DataT, DerivedT> operator*(DataT in) const noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v *= in;
		return v;
	}


	// Operator /
	Vec4<DataT, DerivedT>& operator/=(DataT in) noexcept {
		this->x /= in;
		this->y /= in;
		this->z /= in;
		this->w /= in;
		return *this;
	}

	Vec4<DataT, DerivedT> operator/(DataT in) noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v /= in;
		return v;
	}

	Vec4<DataT, DerivedT> operator/(DataT in) const noexcept {
		Vec4<DataT, DerivedT> v(*this);
		v /= in;
		return v;
	}

	#pragma endregion operators
};


//----------------------------------------------------------------------------------
// Vector aliases
//----------------------------------------------------------------------------------

#if defined(_WIN32)
	#include <DirectXMath.h>

	using int2 = Vec2<int32_t, DirectX::XMINT2>;
	using int3 = Vec3<int32_t, DirectX::XMINT3>;
	using int4 = Vec4<int32_t, DirectX::XMINT4>;

	using uint2 = Vec2<uint32_t, DirectX::XMUINT2>;
	using uint3 = Vec2<uint32_t, DirectX::XMUINT3>;
	using uint4 = Vec2<uint32_t, DirectX::XMUINT4>;

	using float2 = Vec2<float, DirectX::XMFLOAT2>;
	using float3 = Vec3<float, DirectX::XMFLOAT3>;
	using float4 = Vec4<float, DirectX::XMFLOAT4>;

#else
	using int2 = Vec2<int32_t>;
	using int3 = Vec3<int32_t>;
	using int4 = Vec4<int32_t>;

	using uint2 = Vec2<uint32_t>;
	using uint3 = Vec2<uint32_t>;
	using uint4 = Vec2<uint32_t>;

	using float2 = Vec2<float>;
	using float3 = Vec3<float>;
	using float4 = Vec4<float>;

#endif //defined(_WIN32)
