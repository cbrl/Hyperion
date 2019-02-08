#ifndef HLSL_FALSE_COLOR_TEXCOORD
#define HLSL_FALSE_COLOR_TEXCOORD

#include "include/input_structs.hlsli"
#include "forward/forward_include.hlsli"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	return float4(input.uv, 0.0f, 1.0f);
}


#endif //HLSL_FALSE_COLOR_TEXCOORD