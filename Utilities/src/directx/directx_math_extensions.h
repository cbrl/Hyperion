#pragma once

#include "datatypes/datatypes.h"
#include "math/math.h"
#include <utility> //std::pair


// Include the DirectX namespace, which contains
// all of the math related classes and functions
using namespace DirectX;


// Determine if a point is inside a triangle using barycentric coordinates
[[nodiscard]]
inline bool PointInTriangle(const vec3_f32& vert1,
                            const vec3_f32& vert2,
                            const vec3_f32& vert3,
                            const vec3_f32& point) {
	const XMVECTOR v0 = XMLoad(&vert2) - XMLoad(&vert1);
	const XMVECTOR v1 = XMLoad(&vert3) - XMLoad(&vert1);
	const XMVECTOR v2 = XMLoad(&point) - XMLoad(&vert1);

	const f32 d00   = XMVectorGetX(XMVector3Dot(v0, v0));
	const f32 d01   = XMVectorGetX(XMVector3Dot(v0, v1));
	const f32 d11   = XMVectorGetX(XMVector3Dot(v1, v1));
	const f32 d20   = XMVectorGetX(XMVector3Dot(v2, v0));
	const f32 d21   = XMVectorGetX(XMVector3Dot(v2, v1));
	const f32 denom = (d00 * d11) - (d01 * d01);

	const f32 a = ((d11 * d20) - (d01 * d21)) / denom;
	const f32 b = ((d00 * d21) - (d01 * d20)) / denom;
	const f32 y = 1.0f - a - b;

	return 0 <= a && a <= 1 &&
	       0 <= b && b <= 1 &&
	       0 <= y && y <= 1;
}


// Determine if a point is inside a triangle using barycentric coordinates
[[nodiscard]]
inline bool XM_CALLCONV PointInTriangle(FXMVECTOR vert1,
                                        FXMVECTOR vert2,
                                        FXMVECTOR vert3,
                                        CXMVECTOR point) {
	const XMVECTOR v0 = vert2 - vert1;
	const XMVECTOR v1 = vert3 - vert1;
	const XMVECTOR v2 = point - vert1;

	const f32 d00   = XMVectorGetX(XMVector3Dot(v0, v0));
	const f32 d01   = XMVectorGetX(XMVector3Dot(v0, v1));
	const f32 d11   = XMVectorGetX(XMVector3Dot(v1, v1));
	const f32 d20   = XMVectorGetX(XMVector3Dot(v2, v0));
	const f32 d21   = XMVectorGetX(XMVector3Dot(v2, v1));
	const f32 denom = (d00 * d11) - (d01 * d01);

	const f32 a = ((d11 * d20) - (d01 * d21)) / denom;
	const f32 b = ((d00 * d21) - (d01 * d20)) / denom;
	const f32 y = 1.0f - a - b;

	return 0 <= a && a <= 1 &&
	       0 <= b && b <= 1 &&
	       0 <= y && y <= 1;
}


// Find the minimum and maximum points in a vector of vertices
template<typename VertexT>
[[nodiscard]]
std::pair<vec3_f32, vec3_f32> MinMaxPoint(const std::vector<VertexT>& vertices) {
	vec3_f32 min{ std::numeric_limits<float>::max() };
	vec3_f32 max{ std::numeric_limits<float>::lowest() };

	for (const auto& vertex : vertices) {
		min.x = std::fminf(min.x, vertex.position.x);
		min.y = std::fminf(min.y, vertex.position.y);
		min.z = std::fminf(min.z, vertex.position.z);

		max.x = std::fmaxf(max.x, vertex.position.x);
		max.y = std::fmaxf(max.y, vertex.position.y);
		max.z = std::fmaxf(max.z, vertex.position.z);
	}

	return {min, max};
}

[[nodiscard]]
inline std::pair<vec3_f32, vec3_f32> MinMaxPoint(const std::vector<vec3_f32>& vertices) {
	vec3_f32 min{ std::numeric_limits<float>::max() };
	vec3_f32 max{ std::numeric_limits<float>::lowest() };

	for (const vec3_f32& vertex : vertices) {
		min.x = std::fminf(min.x, vertex.x);
		min.y = std::fminf(min.y, vertex.y);
		min.z = std::fminf(min.z, vertex.z);

		max.x = std::fmaxf(max.x, vertex.x);
		max.y = std::fmaxf(max.y, vertex.y);
		max.z = std::fmaxf(max.z, vertex.z);
	}

	return {min, max};
}

[[nodiscard]]
inline std::pair<XMVECTOR, XMVECTOR> MinMaxPoint(const std::vector<XMVECTOR>& vertices) {
	XMVECTOR min{FLT_MAX, FLT_MAX, FLT_MAX};
	XMVECTOR max{-FLT_MAX, -FLT_MAX, -FLT_MAX};

	for (const XMVECTOR& vertex : vertices) {
		min = XMVectorMin(min, vertex);
		max = XMVectorMax(max, vertex);
	}

	return {min, max};
}


// Clamp an angle (radians) to a min and max range (min & max cannot exceed +/-pi)
[[nodiscard]]
inline f32 ClampAngle(f32 angle, f32 min, f32 max) {
	assert(min <= max);
	assert(min >= -XM_PI && min <= XM_PI);
	assert(max >= -XM_PI && max <= XM_PI);

	return std::clamp(std::remainder(angle, XM_2PI), min, max);
}
