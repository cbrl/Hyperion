#ifndef HLSL_MATH
#define HLSL_MATH


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


#endif //HLSL_MATH