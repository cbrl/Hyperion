#ifndef HLSL_TRIANGLE
#define HLSL_TRIANGLE

float4 ClipSpaceTriangle(uint vertex) {
	const float x = (vertex / 2) * 4.0f - 1.0f;
	const float y = (vertex % 2) * 4.0f - 1.0f;
	return float4(x, y, 0.0f, 1.0f);
}

#endif //HLSL_TRIANGLE