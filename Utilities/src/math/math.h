#pragma once

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"
#include <cmath>
#include <limits>


// Convert an rgba vec4 [0.0f, 1.0f] to a hex color value
[[nodiscard]]
inline u32 Float4ColorToU32(const vec4_f32& color) {
	return static_cast<u32>(color[0] * 0xff)            //R
	       | (static_cast<u32>(color[1] * 0xff) << 8)   //G
	       | (static_cast<u32>(color[2] * 0xff) << 16)  //B
	       | (static_cast<u32>(color[3] * 0xff) << 24); //A
}


// Find the minimum and maximum points in a vector of vertices
[[nodiscard]]
inline std::pair<vec3_f32, vec3_f32> MinMaxPoint(const std::vector<vec3_f32>& vertices) {
	vec3_f32 min{std::numeric_limits<float>::max()};
	vec3_f32 max{std::numeric_limits<float>::lowest()};

	for (const vec3_f32& vertex : vertices) {
		min[0] = std::fminf(min[0], vertex[0]);
		min[1] = std::fminf(min[1], vertex[1]);
		min[2] = std::fminf(min[2], vertex[2]);

		max[0] = std::fmaxf(max[0], vertex[0]);
		max[1] = std::fmaxf(max[1], vertex[1]);
		max[2] = std::fmaxf(max[2], vertex[2]);
	}

	return {min, max};
}


// Determine if a point is inside a triangle using barycentric coordinates
[[nodiscard]]
inline bool PointInTriangle(const vec3_f32& vert1,
                            const vec3_f32& vert2,
                            const vec3_f32& vert3,
                            const vec3_f32& point) {
	const XMVECTOR v0 = XMLoad(&vert2) - XMLoad(&vert1);
	const XMVECTOR v1 = XMLoad(&vert3) - XMLoad(&vert1);
	const XMVECTOR v2 = XMLoad(&point) - XMLoad(&vert1);

	const f32 d00 = XMVectorGetX(XMVector3Dot(v0, v0));
	const f32 d01 = XMVectorGetX(XMVector3Dot(v0, v1));
	const f32 d11 = XMVectorGetX(XMVector3Dot(v1, v1));
	const f32 d20 = XMVectorGetX(XMVector3Dot(v2, v0));
	const f32 d21 = XMVectorGetX(XMVector3Dot(v2, v1));
	const f32 denom = (d00 * d11) - (d01 * d01);

	const f32 a = ((d11 * d20) - (d01 * d21)) / denom;
	const f32 b = ((d00 * d21) - (d01 * d20)) / denom;
	const f32 y = 1.0f - a - b;

	return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= y && y <= 1;
}