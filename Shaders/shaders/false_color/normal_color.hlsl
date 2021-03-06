#ifndef HLSL_FALSE_COLOR_NORMAL
#define HLSL_FALSE_COLOR_NORMAL

#include "include/input_structs.hlsli"
#include "forward/forward_include.hlsli"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	float3 norm = GetNormal(input.p.xyz, input.n, input.uv);
	norm = SNormToUNorm(norm);

	return float4(norm, 1.0f);
}


#endif //HLSL_FALSE_COLOR_NORMAL