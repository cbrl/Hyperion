#ifndef HLSL_LIGHT
#define HLSL_LIGHT


#include "shaders\include\global.hlsl"


//----------------------------------------------------------------------------------
//  Structs
//----------------------------------------------------------------------------------

struct DirectionalLight {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float  pad;
};

struct PointLight {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float  range;
	float3 attenuation;
	float  pad;
};

struct SpotLight {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float  range;
	float3 direction;
	float  spot;
	float3 attenuation;
	float  pad;
};

struct Material {
	float4 ambient;
	float4 diffuse;
	float4 specular; //specular.w = specular power
	float4 emissive;
	float  optical_density;
	float  dissolve;
	float2 pad;
};


//----------------------------------------------------------------------------------
//  Constant Buffers
//----------------------------------------------------------------------------------

CONSTANT_BUFFER(LightBuffer, SLOT_CBUFFER_LIGHT) {
	// Lights
	uint directional_light_count;
	uint point_light_count;
	uint spot_light_count;
	uint pad;

	// Fog
	float4 fog_color;
	float  fog_start;
	float  fog_range;
	float2 pad2;
};


//----------------------------------------------------------------------------------
//  Structured Buffers
//----------------------------------------------------------------------------------

STRUCTURED_BUFFER(directional_lights, DirectionalLight, SLOT_SRV_DIRECTIONAL_LIGHTS);

STRUCTURED_BUFFER(point_lights, PointLight, SLOT_SRV_POINT_LIGHTS);

STRUCTURED_BUFFER(spot_lights, SpotLight, SLOT_SRV_SPOT_LIGHTS);



//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight L,
							 float3 normal, float3 toEye,
							 out float4 ambient,
							 out float4 diffuse,
							 out float4 spec) {
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.direction;

	// Add ambient term.
	ambient = mat.ambient * L.ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f) {
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
					   out float4 ambient, out float4 diffuse, out float4 spec) {
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > L.range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.ambient * L.ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f) {
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}

	// Attenuate
	float att = 1.0f / dot(L.attenuation, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
					  out float4 ambient, out float4 diffuse, out float4 spec) {
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > L.range)
		return;

	// Normalize the light vector.
	lightVec /= d;

	// Ambient term.
	ambient = mat.ambient * L.ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if (diffuseFactor > 0.0f) {
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}

	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.attenuation, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}


#endif //HLSL_LIGHT