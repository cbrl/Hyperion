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

	float2() = default;

	float2(float _x, float _y) : XMFLOAT2(_x, _y) {}

	float2(XMFLOAT2 _xy) : XMFLOAT2(_xy) {}

	float2(const float2& _float2) = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	bool operator==(const float2& compare) {
		return (this->x == compare.x &&
				this->y == compare.y);
	}

	bool operator!=(const float2& compare) {
		return !(*this == compare);
	}

	float2 operator+(const float2& in) {
		return float2(this->x + in.x,
					  this->y + in.y);
	}

	float2 operator-(const float2& in) {
		return float2(this->x - in.x,
					  this->y - in.y);
	}

	float2 operator*(const float2& in) {
		return float2(this->x * in.x,
					  this->y * in.y);
	}

	float2 operator/(const float2& in) {
		return float2(this->x / in.x,
					  this->y / in.y);
	}
};



//----------------------------------------------------------------------------------
// float3
//----------------------------------------------------------------------------------

struct float3 : public XMFLOAT3 {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	float3() = default;

	float3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}

	float3(float2 _xy, float _z) : XMFLOAT3(_xy.x, _xy.y, z) {}
	float3(XMFLOAT2 _xy, float _z) : XMFLOAT3(_xy.x, _xy.y, _z) {}

	float3(const float3& _float3) = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	bool operator==(const float3& compare) {
		return (this->x == compare.x &&
				this->y == compare.y &&
				this->z == compare.z);
	}

	bool operator!=(const float3& compare) {
		return !(*this == compare);
	}

	float3 operator+(const float3& in) {
		return float3(this->x + in.x,
					  this->y + in.y,
					  this->z + in.z);
	}

	float3 operator-(const float3& in) {
		return float3(this->x - in.x,
					  this->y - in.y,
					  this->z - in.z);
	}

	float3 operator*(const float3& in) {
		return float3(this->x * in.x,
					  this->y * in.y,
					  this->z * in.z);
	}

	float3 operator/(const float3& in) {
		return float3(this->x / in.x,
					  this->y / in.y,
					  this->z / in.z);
	}
};



//----------------------------------------------------------------------------------
// float4
//----------------------------------------------------------------------------------

struct float4 : public XMFLOAT4 {
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	float4() = default;

	float4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}

	float4(float2 _xy, float _z, float _w) : XMFLOAT4(_xy.x, _xy.y, _z, _w) {}
	float4(XMFLOAT2 _xy, float _z, float _w) : XMFLOAT4(_xy.x, _xy.y, _z, _w) {}

	float4(float3 _xyz, float _w) : XMFLOAT4(_xyz.x, _xyz.y, _xyz.z, _w) {}
	float4(XMFLOAT3 _xyz, float _w) : XMFLOAT4(_xyz.x, _xyz.y, _xyz.z, _w) {}

	float4(const float4& _float4) = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	bool operator==(const float4& compare) {
		return (this->x == compare.x &&
				this->y == compare.y &&
				this->z == compare.z &&
				this->w == compare.w);
	}

	bool operator!=(const float4& compare) {
		return !(*this == compare);
	}

	float4 operator+(const float4& in) {
		return float4(this->x + in.x,
					  this->y + in.y,
					  this->z + in.z,
					  this->w + in.w);
	}

	float4 operator-(const float4& in) {
		return float4(this->x - in.x,
					  this->y - in.y,
					  this->z - in.z,
					  this->w - in.w);
	}

	float4 operator*(const float4& in) {
		return float4(this->x * in.x,
					  this->y * in.y,
					  this->z * in.z,
					  this->w * in.w);
	}

	float4 operator/(const float4& in) {
		return float4(this->x / in.x,
					  this->y / in.y,
					  this->z / in.z,
					  this->w / in.w);
	}
};