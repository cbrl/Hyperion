#ifndef HLSL_MATERIAL
#define HLSL_MATERIAL


struct Material {
	float4 base_color;
	float  roughness;
	float  metalness;
	float2 pad0;
	float3 emissive;
	float  pad1;
	//float2 anisotropy;
};


#endif //HLSL_MATERIAL