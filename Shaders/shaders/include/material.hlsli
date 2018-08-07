#ifndef HLSL_MATERIAL
#define HLSL_MATERIAL


struct Material {
	float4 ambient;
	float4 diffuse;
	float4 specular; //specular.w = specular power
	float4 reflect;
	float4 emissive;
	float  optical_density;
	bool   has_texture;
	bool   reflection_enabled;
};


#endif //HLSL_MATERIAL