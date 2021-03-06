#ifndef HLSL_NORMAL
#define HLSL_NORMAL

#include "include/math.hlsli"


// http://www.thetenthplanet.de/archives/1180
float3x3 ComputeTBN(float3 position, float3 normal, float2 tex) {
	// Get edge vectors of the pixel triangle
	const float3 dp1  = ddx(position);
	const float3 dp2  = ddy(position);
	const float2 duv1 = ddx(tex);
	const float2 duv2 = ddy(tex);

	// Solve the linear system
	const float3 dp2perp = cross(dp2, normal);
	const float3 dp1perp = cross(normal, dp1);
	const float3 T = (dp2perp * duv1.x) + (dp1perp * duv2.x);
	const float3 B = (dp2perp * duv1.y) + (dp1perp * duv2.y);

	// Construct a scale-invariant frame 
	const float invmax = rsqrt(max(dot(T, T), dot(B, B)));

	return float3x3(T * invmax, B * invmax, normal);
}

float3 TransformNormal(float3x3 TBN, float3 normal_map_sample) {
	const float3 tex_normal = UNormToSNorm(normal_map_sample);
	return normalize(mul(tex_normal, TBN));
}

float3 TransformNormal(float3 position, float3 normal, float2 tex, float3 normal_map_sample) {
	const float3x3 TBN = ComputeTBN(position, normal, tex);
	return TransformNormal(TBN, normal_map_sample);
}


#endif //HLSL_NORMAL