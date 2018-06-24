#pragma once

#include "datatypes/datatypes.h"
#include "directx/directx_math_wrapper.h"
#include <utility>


// Include the DirectX namespace, which contains
// all of the math related classes and functions
using namespace DirectX;


// Convert a f32_4 to a hex color value
inline uint32_t Float4ColorToU32(const f32_4& color) {
	return static_cast<uint32_t>(color.x * 0xff)            //R
	       | (static_cast<uint32_t>(color.y * 0xff) << 8)   //G
	       | (static_cast<uint32_t>(color.z * 0xff) << 16)  //B
	       | (static_cast<uint32_t>(color.w * 0xff) << 24); //A
}


// Determine if a point is inside a triangle using barycentric coordinates
inline bool PointInTriangle(const f32_3& vert1,
                            const f32_3& vert2,
                            const f32_3& vert3,
                            const f32_3& point) {
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
std::pair<XMVECTOR, XMVECTOR> MinMaxPoint(const std::vector<VertexT>& vertices) {
	f32_3 min{FLT_MAX, FLT_MAX, FLT_MAX};
	f32_3 max{FLT_MIN, FLT_MIN, FLT_MIN};

	for (const auto& vertex : vertices) {
		min.x = std::fminf(min.x, vertex.position.x);
		min.y = std::fminf(min.y, vertex.position.y);
		min.z = std::fminf(min.z, vertex.position.z);

		max.x = std::fmaxf(max.x, vertex.position.x);
		max.y = std::fmaxf(max.y, vertex.position.y);
		max.z = std::fmaxf(max.z, vertex.position.z);
	}

	return std::pair<XMVECTOR, XMVECTOR>(XMLoad(&min), XMLoad(&max));
}

inline std::pair<XMVECTOR, XMVECTOR> MinMaxPoint(const std::vector<f32_3>& vertices) {
	f32_3 min{FLT_MAX, FLT_MAX, FLT_MAX};
	f32_3 max{FLT_MIN, FLT_MIN, FLT_MIN};

	for (const f32_3& vertex : vertices) {
		min.x = std::fminf(min.x, vertex.x);
		min.y = std::fminf(min.y, vertex.y);
		min.z = std::fminf(min.z, vertex.z);

		max.x = std::fmaxf(max.x, vertex.x);
		max.y = std::fmaxf(max.y, vertex.y);
		max.z = std::fmaxf(max.z, vertex.z);
	}

	return std::pair<XMVECTOR, XMVECTOR>(XMLoad(&min), XMLoad(&max));
}

inline std::pair<XMVECTOR, XMVECTOR> MinMaxPoint(const std::vector<XMVECTOR>& vertices) {
	XMVECTOR min{FLT_MAX, FLT_MAX, FLT_MAX};
	XMVECTOR max{FLT_MIN, FLT_MIN, FLT_MIN};

	for (const XMVECTOR& vertex : vertices) {
		min = XMVectorMin(min, vertex);
		max = XMVectorMax(max, vertex);
	}

	return std::pair<XMVECTOR, XMVECTOR>(min, max);
}
