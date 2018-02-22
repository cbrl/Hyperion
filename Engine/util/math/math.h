#pragma once

#include <DirectXMath.h>
#include "util\math\directxmath\extensions.h"

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
			0 <= y && y <= 1) {
			return true;
		}
		else {
			return false;
		}
	}


	// Find the minimum and maximum points in a vector of vertices
	template<typename VertexT>
	inline std::pair<XMFLOAT3, XMFLOAT3> MinMaxPoint(const std::vector<VertexT>& vertices) {
		XMFLOAT3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
		XMFLOAT3 max = { FLT_MIN, FLT_MIN, FLT_MIN };

		for (const auto& vertex : vertices) {
			min.x = std::fminf(min.x, vertex.position.x);
			min.y = std::fminf(min.y, vertex.position.y);
			min.z = std::fminf(min.z, vertex.position.y);

			max.x = std::fmaxf(max.x, vertex.position.x);
			max.y = std::fmaxf(max.y, vertex.position.y);
			max.z = std::fmaxf(max.z, vertex.position.y);
		}

		return std::pair<XMFLOAT3, XMFLOAT3>(min, max);
	}

	inline std::pair<XMFLOAT3, XMFLOAT3> MinMaxPoint(const std::vector<XMFLOAT3>& vertices) {
		XMFLOAT3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
		XMFLOAT3 max = { FLT_MIN, FLT_MIN, FLT_MIN };

		for (const auto& vertex : vertices) {
			min.x = std::fminf(min.x, vertex.x);
			min.y = std::fminf(min.y, vertex.y);
			min.z = std::fminf(min.z, vertex.y);

			max.x = std::fmaxf(max.x, vertex.x);
			max.y = std::fmaxf(max.y, vertex.y);
			max.z = std::fmaxf(max.z, vertex.y);
		}

		return std::pair<XMFLOAT3, XMFLOAT3>(min, max);
	}
}