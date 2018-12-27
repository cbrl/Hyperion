#ifndef HLSL_MATERIAL
#define HLSL_MATERIAL


struct Material {
	float4 ambient;           //[0.0, 1.0]
	float4 diffuse;           //[0.0, 1.0]
	float4 specular;          //[0.0, 1.0]
	float4 emissive;          //[0.0, 1.0]
	float  spec_scale;        //[0.0, inf)
	float  spec_exponent;     //[1.0, inf)
	float  opacity;           //[0.0, 1.0]
	bool   mirror_surface;
	float  reflectivity;      //[0.0, 1.0]
	bool   has_texture;
	float  pad;
};


#endif //HLSL_MATERIAL