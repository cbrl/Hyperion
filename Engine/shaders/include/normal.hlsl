#ifndef HLSL_NORMAL
#define HLSL_NORMAL


// http://www.thetenthplanet.de/archives/1180
float3x3 ComputeTBN(float3 position, float3 normal, float2 tex) {
	// Get edge vectors of the pixel triangle
	float3 dp1 = ddx(position);
	float3 dp2 = ddy(position);
	float2 duv1 = ddx(tex);
	float2 duv2 = ddx(tex);

	// Solve the linear system
	float3 dp2perp = cross(dp2, normal);
	float3 dp1perp = cross(normal, dp1);
	float3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	float3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	// Construct a scale-invariant frame 
	float invmax = rsqrt(max(dot(T, T), dot(B, B)));

	return float3x3(T * invmax, B * invmax, normal);
}

float3 TransformNormal(float3 position, float3 normal, float2 tex, float3 normal_map_sample) {
	float3 tex_normal = (2.0f * normal_map_sample) - 1.0f;

	float3x3 TBN = ComputeTBN(position, normal, tex);

	return mul(tex_normal, TBN);
}


#endif //HLSL_NORMAL