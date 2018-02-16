#include "shaders\include\syntax.hlsl"
#include "shader\HLSL.h"


//----------------------------------------------------------------------------------
//  Light Structs
//----------------------------------------------------------------------------------

struct PointLight {
	float4 ambient_color;
	float4 diffuse_color;
	float4 specular; //specular.w = specular power
	float3 position;
	float  range;
	float3 attenuation;
	float  pad;
};

struct DirectionalLight {
	float4 ambient_color;
	float4 diffuse_color;
	float3 direction;
	float  pad;
	float3 specular;
};

struct SpotLight {
	float4 ambient_color;
	float4 diffuse_color;
	float4 specular;
	float3 position;
	float  range;
	float3 direction;
	float  spot;
	float3 attenuation;
	float  pad;
};


//----------------------------------------------------------------------------------
//  Constant Buffers
//----------------------------------------------------------------------------------

CONSTANT_BUFFER(LightBuffer, LIGHT_BUFFER_SLOT) {
	uint point_light_count;
	uint directional_light_count;
	uint spot_light_count;
	uint pad;
}


//----------------------------------------------------------------------------------
//  Structured Buffers
//----------------------------------------------------------------------------------

STRUCTURED_BUFFER(point_lights, PointLight, POINT_LIGHT_SLOT);

STRUCTURED_BUFFER(directional_lights, DirectionalLight, DIRECTIONAL_LIGHT_SLOT);

STRUCTURED_BUFFER(spot_lights, SpotLight, SPOT_LIGHT_SLOT);