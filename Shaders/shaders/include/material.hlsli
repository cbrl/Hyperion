#ifndef HLSL_MATERIAL
#define HLSL_MATERIAL


struct Material {
	float4 base_color;
	float  metalness;
	float  roughness;
	float3 emissive;
	float3 pad;
	//float2 anisotropy;
};


#endif //HLSL_MATERIAL