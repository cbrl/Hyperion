#ifndef HLSL_FALSE_COLOR_ROUGHNESS
#define HLSL_FALSE_COLOR_ROUGHNESS

#include "include/input_structs.hlsli"
#include "forward/forward_include.hlsli"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	const float2 params = GetMaterialParams(input.uv);

	return float4(params.x, params.x, params.x, 1.0f);
}


#endif //HLSL_FALSE_COLOR_ROUGHNESS