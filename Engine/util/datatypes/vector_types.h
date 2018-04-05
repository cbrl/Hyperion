#pragma once

#include <DirectXMath.h>


using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;


//----------------------------------------------------------------------------------
// float2
//----------------------------------------------------------------------------------

struct float2 : public XMFLOAT2 {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	float2() noexcept = default;

	float2(float _x, float _y) noexcept : XMFLOAT2(_x, _y) {}

	float2(XMFLOAT2 _xy) noexcept : XMFLOAT2(_xy) {}

	float2(const float2& _float2) noexcept  = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the float2
	float* RawData() { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator == and !=
	bool operator==(const float2& compare) noexcept {
		return (this->x == compare.x &&
				this->y == compare.y);
	}
	bool operator==(const float2& compare) const noexcept {
		return (this->x == compare.x &&
				this->y == compare.y);
	}
	bool operator!=(const float2& compare) noexcept {
		return !(*this == compare);
	}
	bool operator!=(const float2& compare) const noexcept {
		return !(*this == compare);
	}

	// Operator +
	float2 operator+(const float& in) noexcept {
		return float2(this->x + in,
					  this->y + in);
	}
	float2 operator+(const float& in) const noexcept {
		return float2(this->x + in,
					  this->y + in);
	}
	float2 operator+(const float2& in) noexcept {
		return float2(this->x + in.x,
					  this->y + in.y);
	}
	float2 operator+(const float2& in) const noexcept {
		return float2(this->x + in.x,
					  this->y + in.y);
	}


	// Operator -
	float2 operator-(const float& in) noexcept {
		return float2(this->x - in,
					  this->y - in);
	}
	float2 operator-(const float& in) const noexcept {
		return float2(this->x - in,
					  this->y - in);
	}
	float2 operator-(const float2& in) noexcept {
		return float2(this->x - in.x,
					  this->y - in.y);
	}
	float2 operator-(const float2& in) const noexcept {
		return float2(this->x - in.x,
					  this->y - in.y);
	}


	// Operator *
	float2 operator*(const float& in) noexcept {
		return float2(this->x * in,
					  this->y * in);
	}
	float2 operator*(const float& in) const noexcept {
		return float2(this->x * in,
					  this->y * in);
	}
	float2 operator*(const float2& in) noexcept {
		return float2(this->x * in.x,
					  this->y * in.y);
	}
	float2 operator*(const float2& in) const noexcept {
		return float2(this->x * in.x,
					  this->y * in.y);
	}


	// Operator /
	float2 operator/(const float& in) noexcept {
		return float2(this->x / in,
					  this->y / in);
	}
	float2 operator/(const float& in) const noexcept {
		return float2(this->x / in,
					  this->y / in);
	}
	float2 operator/(const float2& in) noexcept {
		return float2(this->x / in.x,
					  this->y / in.y);
	}
	float2 operator/(const float2& in) const noexcept {
		return float2(this->x / in.x,
					  this->y / in.y);
	}

	#pragma endregion operators
};



//----------------------------------------------------------------------------------
// float3
//----------------------------------------------------------------------------------

struct float3 : public XMFLOAT3 {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	float3() noexcept = default;

	float3(float _x, float _y, float _z) noexcept : XMFLOAT3(_x, _y, _z) {}

	float3(float2 _xy, float _z) noexcept : XMFLOAT3(_xy.x, _xy.y, z) {}
	float3(XMFLOAT2 _xy, float _z) noexcept : XMFLOAT3(_xy.x, _xy.y, _z) {}

	float3(const float3& _float3) noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the float3
	float* RawData() { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	#pragma region operators

	// Operator == and !=
	bool operator==(const float3& compare) noexcept {
		return (this->x == compare.x &&
				this->y == compare.y &&
				this->z == compare.z);
	}
	const bool operator==(const float3& compare) const noexcept {
		return (this->x == compare.x &&
				this->y == compare.y &&
				this->z == compare.z);
	}
	bool operator!=(const float3& compare) noexcept {
		return !(*this == compare);
	}
	const bool operator!=(const float3& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	float3 operator+(const float& in) noexcept {
		return float3(this->x + in,
					  this->y + in,
					  this->z + in);
	}
	float3 operator+(const float& in) const noexcept {
		return float3(this->x + in,
					  this->y + in,
					  this->z + in);
	}
	float3 operator+(const float3& in) noexcept {
		return float3(this->x + in.x,
					  this->y + in.y,
					  this->z + in.z);
	}
	float3 operator+(const float3& in) const noexcept {
		return float3(this->x + in.x,
					  this->y + in.y,
					  this->z + in.z);
	}


	// Operator -
	float3 operator-(const float& in) noexcept {
		return float3(this->x - in,
					  this->y - in,
					  this->z - in);
	}
	float3 operator-(const float& in) const noexcept {
		return float3(this->x - in,
					  this->y - in,
					  this->z - in);
	}
	float3 operator-(const float3& in) noexcept {
		return float3(this->x - in.x,
					  this->y - in.y,
					  this->z - in.z);
	}
	float3 operator-(const float3& in) const noexcept {
		return float3(this->x - in.x,
					  this->y - in.y,
					  this->z - in.z);
	}


	// Operator *
	float3 operator*(const float& in) noexcept {
		return float3(this->x * in,
					  this->y * in,
					  this->z * in);
	}
	float3 operator*(const float& in) const noexcept {
		return float3(this->x * in,
					  this->y * in,
					  this->z * in);
	}
	float3 operator*(const float3& in) noexcept {
		return float3(this->x * in.x,
					  this->y * in.y,
					  this->z * in.z);
	}
	float3 operator*(const float3& in) const noexcept {
		return float3(this->x * in.x,
					  this->y * in.y,
					  this->z * in.z);
	}


	// Operator /
	float3 operator/(const float& in) noexcept {
		return float3(this->x / in,
					  this->y / in,
					  this->z / in);
	}
	float3 operator/(const float& in) const noexcept {
		return float3(this->x / in,
					  this->y / in,
					  this->z / in);
	}
	float3 operator/(const float3& in) noexcept {
		return float3(this->x / in.x,
					  this->y / in.y,
					  this->z / in.z);
	}
	float3 operator/(const float3& in) const noexcept {
		return float3(this->x / in.x,
					  this->y / in.y,
					  this->z / in.z);
	}

	#pragma endregion operators
};



//----------------------------------------------------------------------------------
// float4
//----------------------------------------------------------------------------------

struct float4 : public XMFLOAT4 {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	float4() noexcept = default;

	float4(float _x, float _y, float _z, float _w) noexcept : XMFLOAT4(_x, _y, _z, _w) {}

	float4(float2 _xy, float _z, float _w) noexcept : XMFLOAT4(_xy.x, _xy.y, _z, _w) {}
	float4(XMFLOAT2 _xy, float _z, float _w) noexcept : XMFLOAT4(_xy.x, _xy.y, _z, _w) {}

	float4(float3 _xyz, float _w) noexcept : XMFLOAT4(_xyz.x, _xyz.y, _xyz.z, _w) {}
	float4(XMFLOAT3 _xyz, float _w) noexcept : XMFLOAT4(_xyz.x, _xyz.y, _xyz.z, _w) {}

	float4(const float4& _float4) noexcept = default;


	//----------------------------------------------------------------------------------
	// Functions
	//----------------------------------------------------------------------------------

	// Return a pointer to the first element of the float4
	float* RawData() { return &this->x; }


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	
	#pragma region operators

	// Operator == and !=
	bool operator==(const float4& compare) noexcept {
		return (this->x == compare.x &&
				this->y == compare.y &&
				this->z == compare.z &&
				this->w == compare.w);
	}
	const bool operator==(const float4& compare) const noexcept {
		return (this->x == compare.x &&
				this->y == compare.y &&
				this->z == compare.z &&
				this->w == compare.w);
	}
	bool operator!=(const float4& compare) noexcept {
		return !(*this == compare);
	}
	const bool operator!=(const float4& compare) const noexcept {
		return !(*this == compare);
	}


	// Operator +
	float4 operator+(const float& in) noexcept {
		return float4(this->x + in,
					  this->y + in,
					  this->z + in,
					  this->w + in);
	}
	float4 operator+(const float& in) const noexcept {
		return float4(this->x + in,
					  this->y + in,
					  this->z + in,
					  this->w + in);
	}
	float4 operator+(const float4& in) noexcept {
		return float4(this->x + in.x,
					  this->y + in.y,
					  this->z + in.z,
					  this->w + in.w);
	}
	float4 operator+(const float4& in) const noexcept {
		return float4(this->x + in.x,
					  this->y + in.y,
					  this->z + in.z,
					  this->w + in.w);
	}


	// Operator -
	float4 operator-(const float& in) noexcept {
		return float4(this->x - in,
					  this->y - in,
					  this->z - in,
					  this->w - in);
	}
	float4 operator-(const float& in) const noexcept {
		return float4(this->x - in,
					  this->y - in,
					  this->z - in,
					  this->w - in);
	}
	float4 operator-(const float4& in) noexcept {
		return float4(this->x - in.x,
					  this->y - in.y,
					  this->z - in.z,
					  this->w - in.w);
	}
	float4 operator-(const float4& in) const noexcept {
		return float4(this->x - in.x,
					  this->y - in.y,
					  this->z - in.z,
					  this->w - in.w);
	}


	// Operator *
	float4 operator*(const float& in) noexcept {
		return float4(this->x * in,
					  this->y * in,
					  this->z * in,
					  this->w * in);
	}
	float4 operator*(const float& in) const noexcept {
		return float4(this->x * in,
					  this->y * in,
					  this->z * in,
					  this->w * in);
	}
	float4 operator*(const float4& in) noexcept {
		return float4(this->x * in.x,
					  this->y * in.y,
					  this->z * in.z,
					  this->w * in.w);
	}
	float4 operator*(const float4& in) const noexcept {
		return float4(this->x * in.x,
					  this->y * in.y,
					  this->z * in.z,
					  this->w * in.w);
	}


	// Operator /
	float4 operator/(const float& in) noexcept {
		return float4(this->x / in,
					  this->y / in,
					  this->z / in,
					  this->w / in);
	}
	float4 operator/(const float& in) const noexcept {
		return float4(this->x / in,
					  this->y / in,
					  this->z / in,
					  this->w / in);
	}
	float4 operator/(const float4& in) noexcept {
		return float4(this->x / in.x,
					  this->y / in.y,
					  this->z / in.z,
					  this->w / in.w);
	}
	float4 operator/(const float4& in) const noexcept {
		return float4(this->x / in.x,
					  this->y / in.y,
					  this->z / in.z,
					  this->w / in.w);
	}

	#pragma endregion operators
};