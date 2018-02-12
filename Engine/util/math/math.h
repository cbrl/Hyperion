#pragma once

#include <DirectXMath.h>

namespace DirectX {

	// Determine if a point is inside a triangle using barycentric coordinates
	inline bool PointInTriangle(const XMFLOAT3& point, const XMFLOAT3& vert1,
								const XMFLOAT3& vert2, const XMFLOAT3& vert3) {
		XMVECTOR AB = XMLoadFloat3(&vert2) - XMLoadFloat3(&vert1);
		XMVECTOR AC = XMLoadFloat3(&vert3) - XMLoadFloat3(&vert1);

		XMVECTOR PA = XMLoadFloat3(&point) - XMLoadFloat3(&vert1);
		XMVECTOR PB = XMLoadFloat3(&point) - XMLoadFloat3(&vert2);
		XMVECTOR PC = XMLoadFloat3(&point) - XMLoadFloat3(&vert3);

		float area = XMVectorGetX(XMVector3Cross(AB, AC) / 2.0f);
		float a = XMVectorGetX(XMVector3Cross(PB, PC) / (2.0f * area));
		float b = XMVectorGetX(XMVector3Cross(PC, PA) / (2.0f * area));
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


	// Determine if a point is inside a triangle using barycentric coordinates
	inline bool PointInTriangle(const XMVECTOR& point, const XMVECTOR& vert1,
								const XMVECTOR& vert2, const XMVECTOR& vert3) {
		XMVECTOR AB = vert2 - vert1;
		XMVECTOR AC = vert3 - vert1;

		XMVECTOR PA = point - vert1;
		XMVECTOR PB = point - vert2;
		XMVECTOR PC = point - vert3;

		float area = XMVectorGetX(XMVector3Cross(AB, AC) / 2.0f);
		float a = XMVectorGetX(XMVector3Cross(PB, PC) / (2.0f * area));
		float b = XMVectorGetX(XMVector3Cross(PC, PA) / (2.0f * area));
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