#ifndef HLSL_MATH
#define HLSL_MATH


static const float g_pi         = 3.14159265359;
static const float g_2pi        = 6.28318530718;
static const float g_inv_pi     = 0.31830988618;
static const float g_inv_2pi    = 0.15915494309;
static const float g_half_pi    = 1.57079632679;
static const float g_quarter_pi = 0.78539816339;



//----------------------------------------------------------------------------------
// UNormToSNorm
//----------------------------------------------------------------------------------

float UNormToSNorm(float v_in) {
	return (2.0f * v_in) - 1.0f;
}

float2 UNormToSNorm(float2 v_in) {
	return (2.0f * v_in) - 1.0f;
}

float3 UNormToSNorm(float3 v_in) {
	return (2.0f * v_in) - 1.0f;
}

float4 UNormToSNorm(float4 v_in) {
	return (2.0f * v_in) - 1.0f;
}



//----------------------------------------------------------------------------------
// SNormToUNorm
//----------------------------------------------------------------------------------

float SNormToUNorm(float v_in) {
	return (0.5f * v_in) + 0.5f;
}

float2 SNormToUNorm(float2 v_in) {
	return (0.5f * v_in) + 0.5f;
}

float3 SNormToUNorm(float3 v_in) {
	return (0.5f * v_in) + 0.5f;
}

float4 SNormToUNorm(float4 v_in) {
	return (0.5f * v_in) + 0.5f;
}



//----------------------------------------------------------------------------------
// Transformations
//----------------------------------------------------------------------------------

float3 Homogenize(float4 input) {
	return input.xyz / input.w;
}


#endif //HLSL_MATH