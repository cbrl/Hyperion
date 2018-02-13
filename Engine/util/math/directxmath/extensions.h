#pragma once

#include <DirectXMath.h>

namespace DirectX {
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


	inline bool operator==(const XMFLOAT2& first, const XMFLOAT2& second) {
		return (first.x == second.x &&
				first.y == second.y);
	}


	inline bool operator!=(const XMFLOAT2& first, const XMFLOAT2& second) {
		return !(first == second);
	}


	inline bool operator==(const XMFLOAT3& first, const XMFLOAT3& second) {
		return (first.x == second.x &&
				first.y == second.y &&
				first.z == second.z);
	}


	inline bool operator!=(const XMFLOAT3& first, const XMFLOAT3& second) {
		return !(first == second);
	}
}
