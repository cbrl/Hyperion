#pragma once

#include <cmath>
#include <DirectXMath.h>
#include "datatypes/datatypes.h"


using namespace DirectX;


// Convert a float4 to a hex color value
inline uint32_t Float4ColorToU32(const XMFLOAT4& color) {
	return static_cast<uint32_t>(color.x * 0xff) //R
	       | (static_cast<uint32_t>(color.y * 0xff) << 8) //G
	       | (static_cast<uint32_t>(color.z * 0xff) << 16) //B
	       | (static_cast<uint32_t>(color.w * 0xff) << 24); //A
}


// Determine if a point is inside a triangle using barycentric coordinates
inline bool PointInTriangle(const XMFLOAT3& vert1,
                            const XMFLOAT3& vert2,
                            const XMFLOAT3& vert3,
                            const XMFLOAT3& point) {
	const XMVECTOR v0 = XMLoadFloat3(&vert2) - XMLoadFloat3(&vert1);
	const XMVECTOR v1 = XMLoadFloat3(&vert3) - XMLoadFloat3(&vert1);
	const XMVECTOR v2 = XMLoadFloat3(&point) - XMLoadFloat3(&vert1);

	const float d00   = XMVectorGetX(XMVector3Dot(v0, v0));
	const float d01   = XMVectorGetX(XMVector3Dot(v0, v1));
	const float d11   = XMVectorGetX(XMVector3Dot(v1, v1));
	const float d20   = XMVectorGetX(XMVector3Dot(v2, v0));
	const float d21   = XMVectorGetX(XMVector3Dot(v2, v1));
	const float denom = (d00 * d11) - (d01 * d01);

	const float a = ((d11 * d20) - (d01 * d21)) / denom;
	const float b = ((d00 * d21) - (d01 * d20)) / denom;
	const float y = 1.0f - a - b;

	return 0 <= a && a <= 1 &&
	       0 <= b && b <= 1 &&
	       0 <= y && y <= 1;
}


// Determine if a point is inside a triangle using barycentric coordinates
inline bool XM_CALLCONV PointInTriangle(FXMVECTOR vert1,
                                        FXMVECTOR vert2,
                                        FXMVECTOR vert3,
                                        CXMVECTOR point) {
	const XMVECTOR v0 = vert2 - vert1;
	const XMVECTOR v1 = vert3 - vert1;
	const XMVECTOR v2 = point - vert1;

	const float d00   = XMVectorGetX(XMVector3Dot(v0, v0));
	const float d01   = XMVectorGetX(XMVector3Dot(v0, v1));
	const float d11   = XMVectorGetX(XMVector3Dot(v1, v1));
	const float d20   = XMVectorGetX(XMVector3Dot(v2, v0));
	const float d21   = XMVectorGetX(XMVector3Dot(v2, v1));
	const float denom = (d00 * d11) - (d01 * d01);

	const float a = ((d11 * d20) - (d01 * d21)) / denom;
	const float b = ((d00 * d21) - (d01 * d20)) / denom;
	const float y = 1.0f - a - b;

	return 0 <= a && a <= 1 &&
	       0 <= b && b <= 1 &&
	       0 <= y && y <= 1;
}


// Find the minimum and maximum points in a vector of vertices
template<typename VertexT>
std::pair<XMVECTOR, XMVECTOR> MinMaxPoint(const std::vector<VertexT>& vertices) {
	float3 min = {FLT_MAX, FLT_MAX, FLT_MAX};
	float3 max = {FLT_MIN, FLT_MIN, FLT_MIN};

	for (const auto& vertex : vertices) {
		min.x = std::fminf(min.x, vertex.position.x);
		min.y = std::fminf(min.y, vertex.position.y);
		min.z = std::fminf(min.z, vertex.position.z);

		max.x = std::fmaxf(max.x, vertex.position.x);
		max.y = std::fmaxf(max.y, vertex.position.y);
		max.z = std::fmaxf(max.z, vertex.position.z);
	}

	return std::pair<XMVECTOR, XMVECTOR>(XMLoadFloat3(&min), XMLoadFloat3(&max));
}

inline std::pair<XMVECTOR, XMVECTOR> MinMaxPoint(const std::vector<float3>& vertices) {
	float3 min = {FLT_MAX, FLT_MAX, FLT_MAX};
	float3 max = {FLT_MIN, FLT_MIN, FLT_MIN};

	for (const float3& vertex : vertices) {
		min.x = std::fminf(min.x, vertex.x);
		min.y = std::fminf(min.y, vertex.y);
		min.z = std::fminf(min.z, vertex.z);

		max.x = std::fmaxf(max.x, vertex.x);
		max.y = std::fmaxf(max.y, vertex.y);
		max.z = std::fmaxf(max.z, vertex.z);
	}

	return std::pair<XMVECTOR, XMVECTOR>(XMLoadFloat3(&min), XMLoadFloat3(&max));
}

inline std::pair<XMVECTOR, XMVECTOR> MinMaxPoint(const std::vector<XMVECTOR>& vertices) {
	XMVECTOR min = {FLT_MAX, FLT_MAX, FLT_MAX};
	XMVECTOR max = {FLT_MIN, FLT_MIN, FLT_MIN};

	for (const XMVECTOR& vertex : vertices) {
		min = XMVectorMin(min, vertex);
		max = XMVectorMax(max, vertex);
	}

	return std::pair<XMVECTOR, XMVECTOR>(min, max);
}
