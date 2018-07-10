#ifndef HLSL_FALSE_COLOR_NORMAL
#define HLSL_FALSE_COLOR_NORMAL

#include "include/input_structs.hlsl"
#include "include/global.hlsl"
#include "include/normal.hlsl"


Texture2D normal_map : REG_T(SLOT_SRV_NORMAL);


float4 PS(PSPositionNormalTexture input) : SV_Target {

	const float3 norm_sample = normal_map.Sample(linear_wrap, input.tex);

	float3 norm;
	if (norm_sample.z) {
		norm = TransformNormal(input.position, input.normal, input.tex, norm_sample);
	}
	else {
		norm = input.normal;
	}

	norm = (0.5f * norm) + 0.5f;

	return float4(norm, 1.0f);
}


#endif //HLSL_FALSE_COLOR_NORMAL