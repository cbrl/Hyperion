#ifndef HLSL_FALSE_COLOR_DEPTH
#define HLSL_FALSE_COLOR_DEPTH

#include "include/input_structs.hlsl"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	float3 color = { input.position.z, input.position.z, input.position.z };
	color = (color * 100.0f) - 99.0f;
	color = saturate(color);

	return float4(color, 1.0f);
}


#endif //HLSL_FALSE_COLOR_DEPTH