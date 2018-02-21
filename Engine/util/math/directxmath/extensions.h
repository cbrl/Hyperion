#pragma once

#include <DirectXMath.h>

namespace DirectX {
	//----------------------------------------------------------------------------------
	// XMFLOAT2
	//----------------------------------------------------------------------------------

	inline bool operator==(const XMFLOAT2& first, const XMFLOAT2& second) {
		return (first.x == second.x &&
				first.y == second.y);
	}

	inline bool operator!=(const XMFLOAT2& first, const XMFLOAT2& second) {
		return !(first == second);
	}

	inline XMFLOAT2 operator+(const XMFLOAT2& first, const XMFLOAT2& second) {
		XMFLOAT2 out;
		out.x = first.x + second.x;
		out.y = first.y + second.y;
		return out;
	}

	inline XMFLOAT2 operator-(const XMFLOAT2& first, const XMFLOAT2& second) {
		XMFLOAT2 out;
		out.x = first.x - second.x;
		out.y = first.y - second.y;
		return out;
	}

	inline XMFLOAT2 operator*(const XMFLOAT2& first, const XMFLOAT2& second) {
		XMFLOAT2 out;
		out.x = first.x * second.x;
		out.y = first.y * second.y;
		return out;
	}

	inline XMFLOAT2 operator/(const XMFLOAT2& first, const XMFLOAT2& second) {
		XMFLOAT2 out;
		out.x = first.x / second.x;
		out.y = first.y / second.y;
		return out;
	}



	//----------------------------------------------------------------------------------
	// XMFLOAT3
	//----------------------------------------------------------------------------------

	inline bool operator==(const XMFLOAT3& first, const XMFLOAT3& second) {
		return (first.x == second.x &&
				first.y == second.y &&
				first.z == second.z);
	}

	inline bool operator!=(const XMFLOAT3& first, const XMFLOAT3& second) {
		return !(first == second);
	}

	inline XMFLOAT3 operator+(const XMFLOAT3& first, const XMFLOAT3& second) {
		XMFLOAT3 out;
		out.x = first.x + second.x;
		out.y = first.y + second.y;
		out.z = first.z + second.z;
		return out;
	}

	inline XMFLOAT3 operator-(const XMFLOAT3& first, const XMFLOAT3& second) {
		XMFLOAT3 out;
		out.x = first.x - second.x;
		out.y = first.y - second.y;
		out.z = first.z - second.z;
		return out;
	}

	inline XMFLOAT3 operator*(const XMFLOAT3& first, const XMFLOAT3& second) {
		XMFLOAT3 out;
		out.x = first.x * second.x;
		out.y = first.y * second.y;
		out.z = first.z * second.z;
		return out;
	}

	inline XMFLOAT3 operator/(const XMFLOAT3& first, const XMFLOAT3& second) {
		XMFLOAT3 out;
		out.x = first.x / second.x;
		out.y = first.y / second.y;
		out.z = first.z / second.z;
		return out;
	}



	//----------------------------------------------------------------------------------
	// XMFLOAT4
	//----------------------------------------------------------------------------------

	inline bool operator==(const XMFLOAT4& first, const XMFLOAT4& second) {
		return (first.x == second.x &&
				first.y == second.y &&
				first.z == second.z &&
				first.w == second.w);
	}

	inline bool operator!=(const XMFLOAT4& first, const XMFLOAT4& second) {
		return !(first == second);
	}

	inline XMFLOAT4 operator+(const XMFLOAT4& first, const XMFLOAT4& second) {
		XMFLOAT4 out;
		out.x = first.x + second.x;
		out.y = first.y + second.y;
		out.z = first.z + second.z;
		out.w = first.w + second.w;
		return out;
	}

	inline XMFLOAT4 operator-(const XMFLOAT4& first, const XMFLOAT4& second) {
		XMFLOAT4 out;
		out.x = first.x - second.x;
		out.y = first.y - second.y;
		out.z = first.z - second.z;
		out.w = first.w - second.w;
		return out;
	}

	inline XMFLOAT4 operator*(const XMFLOAT4& first, const XMFLOAT4& second) {
		XMFLOAT4 out;
		out.x = first.x * second.x;
		out.y = first.y * second.y;
		out.z = first.z * second.z;
		out.w = first.w * second.w;
		return out;
	}

	inline XMFLOAT4 operator/(const XMFLOAT4& first, const XMFLOAT4& second) {
		XMFLOAT4 out;
		out.x = first.x / second.x;
		out.y = first.y / second.y;
		out.z = first.z / second.z;
		out.w = first.w / second.w;
		return out;
	}
}
