#ifndef HLSL_FALSE_COLOR_POSITION
#define HLSL_FALSE_COLOR_POSITION

#include "include/input_structs.hlsli"


float4 PS(PSPositionNormalTexture input) : SV_Target{

	float3 color = (0.5f * input.position_world) + 0.5f;

	return float4(normalize(color), 1.0f);
}


#endif //HLSL_FALSE_COLOR_POSITION