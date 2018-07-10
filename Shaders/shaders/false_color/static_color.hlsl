#ifndef HLSL_FALSE_COLOR_STATIC
#define HLSL_FALSE_COLOR_STATIC

#include "include/input_structs.hlsl"
#include "include/syntax.hlsl"
#include "hlsl.h"

cbuffer Color : REG_B(SLOT_CBUFFER_COLOR) {
	float4 color;
}

float4 PS(PSPositionNormalTexture input) : SV_Target {

	return color;
}

#endif //HLSL_FALSE_COLOR_STATIC