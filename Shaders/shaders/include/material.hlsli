#ifndef HLSL_MATERIAL
#define HLSL_MATERIAL


struct Material {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emissive;
	float  spec_scale;
	float  spec_exponent;
	float  opacity;
	float  refraction_index;
	bool   has_texture;
	bool   reflection_enabled;
	float2 pad;
};


#endif //HLSL_MATERIAL