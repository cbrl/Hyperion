#ifndef HLSL_FALSE_COLOR_DEPTH
#define HLSL_FALSE_COLOR_DEPTH

#include "include/input_structs.hlsl"


float4 PS(PSPositionNormalTexture input) : SV_Target {

	return float4(input.position.z, input.position.z, input.position.z, 1.0f);
}


#endif //HLSL_FALSE_COLOR_DEPTH