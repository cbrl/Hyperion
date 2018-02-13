#pragma once

#include <DirectXMath.h>

namespace DirectX {

	// Determine if a point is inside a triangle using barycentric coordinates
	inline bool PointInTriangle(const XMFLOAT3& point, const XMFLOAT3& vert1,
								const XMFLOAT3& vert2, const XMFLOAT3& vert3) {
		XMVECTOR v0 = XMLoadFloat3(&vert2) - XMLoadFloat3(&vert1);
		XMVECTOR v1 = XMLoadFloat3(&vert3) - XMLoadFloat3(&vert1);
		XMVECTOR v2 = XMLoadFloat3(&point) - XMLoadFloat3(&vert1);

		float d00 = XMVectorGetX(XMVector3Dot(v0, v0));
		float d01 = XMVectorGetX(XMVector3Dot(v0, v1));
		float d11 = XMVectorGetX(XMVector3Dot(v1, v1));
		float d20 = XMVectorGetX(XMVector3Dot(v2, v0));
		float d21 = XMVectorGetX(XMVector3Dot(v2, v1));
		float denom = (d00 * d11) - (d01 * d01);

		float a = ((d11 * d20) - (d01 * d21)) / denom;
		float b = ((d00 * d21) - (d01 * d20)) / denom;
		float y = 1.0f - a - b;

		if (0 <= a && a <= 1 &&
			0 <= b && b <= 1 &&
			0 <= y && y <= 1) {
			return true;
		}
		else {
			return false;
		}
	}


	// Determine if a point is inside a triangle using barycentric coordinates
	inline bool PointInTriangle(const XMVECTOR& point, const XMVECTOR& vert1,
								const XMVECTOR& vert2, const XMVECTOR& vert3) {
		XMVECTOR v0 = vert2 - vert1;
		XMVECTOR v1 = vert3 - vert1;
		XMVECTOR v2 = point - vert1;

		float d00 = XMVectorGetX(XMVector3Dot(v0, v0));
		float d01 = XMVectorGetX(XMVector3Dot(v0, v1));
		float d11 = XMVectorGetX(XMVector3Dot(v1, v1));
		float d20 = XMVectorGetX(XMVector3Dot(v2, v0));
		float d21 = XMVectorGetX(XMVector3Dot(v2, v1));
		float denom = (d00 * d11) - (d01 * d01);

		float a = ((d11 * d20) - (d01 * d21)) / denom;
		float b = ((d00 * d21) - (d01 * d20)) / denom;
		float y = 1.0f - a - b;

		if (0 <= a && a <= 1 &&
			0 <= b && b <= 1 &&
			0 <= y && y <= 1)
		{
			return true;
		}
		else {
			return false;
		}
	}
}