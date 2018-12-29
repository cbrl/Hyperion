#ifndef HLSL_FALSE_COLOR_DEPTH
#define HLSL_FALSE_COLOR_DEPTH

#include "include/input_structs.hlsli"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	float3 color = { input.p.z, input.p.z, input.p.z };
	color /= input.p.w;
	color  = saturate(color);

	return float4(color, 1.0f);
}


#endif //HLSL_FALSE_COLOR_DEPTH