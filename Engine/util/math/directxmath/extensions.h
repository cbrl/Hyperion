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


	inline bool XMFloat2Equal(const XMFLOAT2& first, const XMFLOAT2& second) {
		return (first.x == second.x &&
				first.y == second.y);
	}


	inline bool XMFloat3Equal(const XMFLOAT3& first, const XMFLOAT3& second) {
		return (first.x == second.x &&
				first.y == second.y &&
				first.z == second.z);
	}
}