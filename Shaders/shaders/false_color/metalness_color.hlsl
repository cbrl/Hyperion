#ifndef HLSL_FALSE_COLOR_METALNESS
#define HLSL_FALSE_COLOR_METALNESS

#include "include/input_structs.hlsli"
#include "forward/forward_include.hlsli"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	const float2 params = GetMaterialParams(input.uv);

	return float4(params.y, params.y, params.y, 1.0f);
}


#endif //HLSL_FALSE_COLOR_METALNESS