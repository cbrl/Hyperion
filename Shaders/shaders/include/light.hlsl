#ifndef HLSL_LIGHT
#define HLSL_LIGHT

#include "include\global.hlsl"


//----------------------------------------------------------------------------------
//  Defines
//----------------------------------------------------------------------------------
#define SPECULAR_FACTOR_FUNC SpecularBlinnPhong


//----------------------------------------------------------------------------------
//  Structs
//----------------------------------------------------------------------------------

struct Material {
	float4 ambient;
	float4 diffuse;
	float4 specular; //specular.w = specular power
	float4 reflect;
	float4 emissive;
	float  optical_density;
	float  dissolve;
	bool   has_texture;
	bool   reflection_enabled;
};


struct DirectionalLight {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float  pad;
};

struct ShadowedDirectionalLight {
	DirectionalLight light;
	matrix world_to_projection;
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

struct ShadowedPointLight {
	PointLight light;
	matrix world_to_light;
	float2 projection_values;
	float2 pad;
};


struct SpotLight {
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float  range;
	float3 direction;
	float  cos_umbra;
	float  cos_penumbra;
	float3 attenuation;
};

struct ShadowedSpotLight {
	SpotLight light;
	matrix world_to_projection;
};


struct ShadowMap {
	SamplerComparisonState sam_shadow;
	Texture2DArray maps;
	uint index;
};

struct ShadowCubeMap {
	SamplerComparisonState sam_shadow;
	TextureCubeArray maps;
	uint index;
};


//----------------------------------------------------------------------------------
//  Constant Buffers
//----------------------------------------------------------------------------------

CONSTANT_BUFFER(LightBuffer, SLOT_CBUFFER_LIGHT) {
	// Lights
	uint num_directional_lights;
	uint num_point_lights;
	uint num_spot_lights;
	uint lb_pad0;

	// Shadowed Lights
	uint num_shadow_directional_lights;
	uint num_shadow_point_lights;
	uint num_shadow_spot_lights;
	uint lb_pad1;

	// Fog
	float4 fog_color;
	float  fog_start;
	float  fog_range;
	float2 lb_pad2;
};


//----------------------------------------------------------------------------------
//  Structured Buffers
//----------------------------------------------------------------------------------

// Lights
STRUCTURED_BUFFER(directional_lights, DirectionalLight, SLOT_SRV_DIRECTIONAL_LIGHTS);
STRUCTURED_BUFFER(point_lights,       PointLight,       SLOT_SRV_POINT_LIGHTS);
STRUCTURED_BUFFER(spot_lights,        SpotLight,        SLOT_SRV_SPOT_LIGHTS);


// Shadowed Lights
STRUCTURED_BUFFER(shadow_directional_lights, ShadowedDirectionalLight, SLOT_SRV_DIRECTIONAL_LIGHTS_SHADOWED);
TEXTURE_2D_ARRAY(directional_light_smaps, SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS);

STRUCTURED_BUFFER(shadow_point_lights, ShadowedPointLight, SLOT_SRV_POINT_LIGHTS_SHADOWED);
TEXTURE_CUBE_ARRAY(point_light_smaps, SLOT_SRV_POINT_LIGHT_SHADOW_MAPS);

STRUCTURED_BUFFER(shadow_spot_lights, ShadowedSpotLight, SLOT_SRV_SPOT_LIGHTS_SHADOWED);
TEXTURE_2D_ARRAY(spot_light_smaps, SLOT_SRV_SPOT_LIGHT_SHADOW_MAPS);



//----------------------------------------------------------------------------------
// DiffuseFactor
//----------------------------------------------------------------------------------
// L: light vector (surface to light)
// N: normal vector
//----------------------------------------------------------------------------------
float DiffuseFactor(float3 L, float3 N) {
	return max(dot(N, L), 0.0f);
}


//----------------------------------------------------------------------------------
// Specular Factor Functions
//----------------------------------------------------------------------------------
//     L: light vector (surface to light)
//     N: normal vector
//     V: view vector (surface to camera)
// power: specular exponent
//----------------------------------------------------------------------------------
float SpecularPhong(float3 L, float3 N, float3 V, float power) {
	const float3 H       = normalize(L + V);
	const float  N_dot_H = max(dot(N, H), 0.0f);
	return pow(N_dot_H, power);
}

float SpecularBlinnPhong(float3 L, float3 N, float3 V, float power) {
	const float3 R       = normalize(reflect(-L, N));
	const float  R_dot_V = max(dot(R, V), 0.0f);
	return pow(R_dot_V, power);
}


//----------------------------------------------------------------------------------
// Attenuation
//----------------------------------------------------------------------------------
//          d: distance to light
// att_values: constant, linear, and quadratic attenuation values respectively
//----------------------------------------------------------------------------------
float Attenuation(float d, float3 att_values) {
	return 1.0f / dot(att_values, float3(1.0f, d, d*d));
}


//----------------------------------------------------------------------------------
// SpotIntensity
//----------------------------------------------------------------------------------
//         L: light vector (surface to light)
//         D: light direction
// cos_theta: cosine of the umbra angle
//   cos_phi: cosine of the penumbra angle
//----------------------------------------------------------------------------------
float SpotIntensity(float3 L, float3 D, float cos_theta, float cos_phi) {
	const float alpha = dot(-L, D);
	return smoothstep(cos_phi, cos_theta, alpha);
}


//----------------------------------------------------------------------------------
// ShadowFactor
//----------------------------------------------------------------------------------
// shadow_map: shadow map for this light
//      p_ndc: position vector in light NDC space
//----------------------------------------------------------------------------------
float ShadowFactor(ShadowMap shadow_map, float3 p_ndc) {
	const float2 uv       = float2(0.5f, -0.5f) * p_ndc.xy + 0.5f;
	const float3 location = { uv, shadow_map.index };

	return shadow_map.maps.SampleCmpLevelZero(shadow_map.sam_shadow, location, p_ndc.z);
}


//----------------------------------------------------------------------------------
// ShadowFactor
//----------------------------------------------------------------------------------
//        shadow_map: shadow cube map for this light
//           p_light: position vector in light space
// projection_values: projection values of the light
//----------------------------------------------------------------------------------
float ShadowFactor(ShadowCubeMap shadow_map, float3 p_light, float2 projection_values) {
	const float3 p_light_abs = abs(p_light);

	const float  p_light_z = max(p_light_abs.x, max(p_light_abs.y, p_light_abs.z));
	const float  p_ndc_z   = projection_values.x + projection_values.y / p_light_z;
	const float4 location  = float4(p_light, shadow_map.index);

	return shadow_map.maps.SampleCmpLevelZero(shadow_map.sam_shadow, location, p_ndc_z);
}



//----------------------------------------------------------------------------------
// ComputeDirectionalLight
//----------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//----------------------------------------------------------------------------------
void ComputeDirectionalLight(DirectionalLight L,
							 Material mat,
							 float3 normal,
							 float3 view_vec,
							 out float4 ambient,
							 out float4 diffuse,
							 out float4 specular) {
	// Initialize outputs.
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 light_vec = -L.direction;

	// Add ambient term.
	ambient = mat.ambient * L.ambient;

	// Diffuse
	const float diffuse_factor = DiffuseFactor(light_vec, normal);
	diffuse = diffuse_factor * mat.diffuse * L.diffuse;

	// Specular
	const float specular_factor = SPECULAR_FACTOR_FUNC(light_vec, normal, view_vec, mat.specular.w);
	specular = specular_factor * L.specular * mat.specular;
}


void ComputeShadowedDirectionalLight(ShadowedDirectionalLight L,
									 ShadowMap shadow_map,
									 Material mat,
									 float3 pos,
									 float3 normal,
									 float3 view_vec,
									 out float4 ambient,
									 out float4 diffuse,
									 out float4 specular) {

	float4 ambient0;
	float4 diffuse0;
	float4 specular0;

	ComputeDirectionalLight(L.light, mat, normal, view_vec, ambient0, diffuse0, specular0);

	const float4 p_proj = mul(float4(pos, 1.0f), L.world_to_projection);
	const float3 p_ndc  = p_proj.xyz / p_proj.w;

	const float shadow_factor = ShadowFactor(shadow_map, p_ndc);

	ambient  = ambient0;
	diffuse  = diffuse0  * shadow_factor;
	specular = specular0 * shadow_factor;
}



//----------------------------------------------------------------------------------
// ComputePointLight
//----------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//----------------------------------------------------------------------------------
void ComputePointLight(PointLight L,
					   Material mat,
					   float3 pos,
					   float3 normal,
					   float3 view_vec,
					   out float4 ambient,
					   out float4 diffuse,
					   out float4 specular) {

	// Initialize outputs.
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 light_vec = L.position - pos;

	// The distance from surface to light.
	float d = length(light_vec);

	// Range test.
	if (d > L.range)
		return;

	// Normalize the light vector.
	light_vec /= d;

	// Ambient term.
	ambient = mat.ambient * L.ambient;

	// Diffuse
	const float diffuse_factor = DiffuseFactor(light_vec, normal);
	diffuse = diffuse_factor * mat.diffuse * L.diffuse;

	// Specular
	const float specular_factor = SPECULAR_FACTOR_FUNC(light_vec, normal, view_vec, mat.specular.w);
	specular = specular_factor * L.specular * mat.specular;

	// Attenution
	const float attenuation = Attenuation(d, L.attenuation);

	diffuse  *= attenuation;
	specular *= attenuation;
}


void ComputeShadowedPointLight(ShadowedPointLight L,
							   ShadowCubeMap shadow_map,
							   Material mat,
							   float3 pos,
							   float3 normal,
							   float3 view_vec,
							   out float4 ambient,
							   out float4 diffuse,
							   out float4 specular) {

	float4 ambient0;
	float4 diffuse0;
	float4 specular0;

	ComputePointLight(L.light, mat, pos, normal, view_vec, ambient0, diffuse0, specular0);

	const float3 p_light      = mul(float4(pos, 1.0f), L.world_to_light).xyz;
	const float shadow_factor = ShadowFactor(shadow_map, p_light, L.projection_values);

	ambient  = ambient0;
	diffuse  = diffuse0  * shadow_factor;
	specular = specular0 * shadow_factor;
}



//----------------------------------------------------------------------------------
// ComputeSpotLight
//----------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//----------------------------------------------------------------------------------
void ComputeSpotLight(SpotLight L,
					  Material mat,
					  float3 pos,
					  float3 normal,
					  float3 view_vec,
					  out float4 ambient,
					  out float4 diffuse,
					  out float4 specular) {

	// Initialize outputs.
	ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 light_vec = L.position - pos;

	// The distance from surface to light.
	float d = length(light_vec);

	// Range test.
	if (d > L.range)
		return;

	// Normalize the light vector.
	light_vec /= d;


	// Ambient
	ambient = mat.ambient * L.ambient;

	// Diffuse
	const float diffuse_factor = DiffuseFactor(light_vec, normal);
	diffuse = diffuse_factor * mat.diffuse * L.diffuse;

	// Specular
	const float specular_factor = SPECULAR_FACTOR_FUNC(light_vec, normal, view_vec, mat.specular.w);
	specular = specular_factor * L.specular * mat.specular;

	// Attenution
	const float attenuation = Attenuation(d, L.attenuation);

	// Spot intensity
	const float intensity = SpotIntensity(light_vec, L.direction, L.cos_umbra, L.cos_penumbra);

	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;
}


void ComputeShadowedSpotLight(ShadowedSpotLight L,
							  ShadowMap shadow_map,
							  Material mat,
							  float3 pos,
							  float3 normal,
							  float3 view_vec,
							  out float4 ambient,
							  out float4 diffuse,
							  out float4 specular) {
	
	float4 ambient0;
	float4 diffuse0;
	float4 specular0;

	ComputeSpotLight(L.light, mat, pos, normal, view_vec, ambient0, diffuse0, specular0);

	const float4 p_proj       = mul(float4(pos, 1.0f), L.world_to_projection);
	const float3 p_ndc        = p_proj.xyz / p_proj.w;
	const float shadow_factor = ShadowFactor(shadow_map, p_ndc);

	ambient  = ambient0;
	diffuse  = diffuse0  * shadow_factor;
	specular = specular0 * shadow_factor;
}


#endif //HLSL_LIGHT