#ifndef HLSL_FALSE_COLOR_DEPTH
#define HLSL_FALSE_COLOR_DEPTH

#include "include/input_structs.hlsli"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	// Range of position.z = [0.0f, 1.0f]
	float3 color = { input.p.z, input.p.z, input.p.z };

	color = (color * 100.0f) - 99.0f;
	color = saturate(color);

	return float4(color, 1.0f);
}


#endif //HLSL_FALSE_COLOR_DEPTH