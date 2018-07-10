#ifndef HLSL_FALSE_COLOR_NORMAL
#define HLSL_FALSE_COLOR_NORMAL

#include "include/input_structs.hlsl"
#include "forward/forward_include.hlsl"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	float3 norm = GetNormal(input.position, input.normal, input.tex);
	norm = (0.5f * norm) + 0.5f;

	return float4(norm, 1.0f);
}


#endif //HLSL_FALSE_COLOR_NORMAL