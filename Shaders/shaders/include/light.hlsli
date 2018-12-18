#ifndef HLSL_LIGHT
#define HLSL_LIGHT

#include "include/global.hlsli"
#include "include/material.hlsli"


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define SPECULAR_FACTOR_FUNC SpecularBlinnPhong


//----------------------------------------------------------------------------------
//  Structs
//----------------------------------------------------------------------------------

struct DirectionalLight {
	float3 diffuse;
	float  pad1;
	float3 specular;
	float  pad2;
	float3 direction;
	float  pad3;
	matrix world_to_projection;
};


struct PointLight {
	float3 diffuse;
	float  pad1;
	float3 specular;
	float  pad2;
	float3 position;
	float  range;
	float3 attenuation;
	float  pad3;
};

struct ShadowPointLight {
	PointLight light;
	matrix world_to_light;
	float2 projection_values;
	float2 pad;
};


struct SpotLight {
	float3 diffuse;
	float  pad1;
	float3 specular;
	float  pad2;
	float3 position;
	float  range;
	float3 direction;
	float  cos_umbra;
	float  cos_penumbra;
	float3 attenuation;
};

struct ShadowSpotLight {
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
//  Structured Buffers
//----------------------------------------------------------------------------------

// Lights
StructuredBuffer<DirectionalLight> g_directional_lights : REG_T(SLOT_SRV_DIRECTIONAL_LIGHTS);
StructuredBuffer<PointLight>       g_point_lights       : REG_T(SLOT_SRV_POINT_LIGHTS);
StructuredBuffer<SpotLight>        g_spot_lights        : REG_T(SLOT_SRV_SPOT_LIGHTS);


// Shadow Lights
StructuredBuffer<DirectionalLight> g_shadow_directional_lights : REG_T(SLOT_SRV_DIRECTIONAL_LIGHTS_SHADOW);
Texture2DArray g_directional_light_smaps : REG_T(SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS);

StructuredBuffer<ShadowPointLight> g_shadow_point_lights : REG_T(SLOT_SRV_POINT_LIGHTS_SHADOW);
TextureCubeArray g_point_light_smaps : REG_T(SLOT_SRV_POINT_LIGHT_SHADOW_MAPS);

StructuredBuffer<ShadowSpotLight> g_shadow_spot_lights : REG_T(SLOT_SRV_SPOT_LIGHTS_SHADOW);
Texture2DArray g_spot_light_smaps : REG_T(SLOT_SRV_SPOT_LIGHT_SHADOW_MAPS);



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
// Computes the diffuse and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//----------------------------------------------------------------------------------
void ComputeDirectionalLight(DirectionalLight L,
                             float3 pos,
                             float3 normal,
                             float3 view_vec,
                             float  spec_exponent,
                             out float3 out_diffuse,
                             out float3 out_specular,
                             out float3 out_p_ndc) {
	// Initialize outputs.
	out_diffuse = float3(0.0f, 0.0f, 0.0f);
	out_specular = float3(0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	const float3 light_vec = -L.direction;

	const float4 p_proj = mul(float4(pos, 1.0f), L.world_to_projection);
	out_p_ndc = p_proj.xyz / p_proj.w;

	// Diffuse
	const float diffuse_factor = DiffuseFactor(light_vec, normal);
	out_diffuse = (any(1.0f < abs(out_p_ndc)) || 0.0f > out_p_ndc.z) ? 0.0f : (diffuse_factor * L.diffuse);

	// Specular
	const float specular_factor = SPECULAR_FACTOR_FUNC(light_vec, normal, view_vec, spec_exponent);
	out_specular = specular_factor * L.specular;
}


void ComputeDirectionalLight(DirectionalLight L,
                             float3 pos,
                             float3 normal,
                             float3 view_vec,
                             float  spec_exponent,
                             out float3 out_diffuse,
                             out float3 out_specular) {

	float3 p_ndc;
	ComputeDirectionalLight(L, pos, normal, view_vec, spec_exponent, out_diffuse, out_specular, p_ndc);
}


void ComputeShadowDirectionalLight(DirectionalLight L,
                                   ShadowMap shadow_map,
                                   float3 pos,
                                   float3 normal,
                                   float3 view_vec,
                                   float spec_exponent,
                                   out float3 out_diffuse,
                                   out float3 out_specular) {

	float3 diffuse0;
	float3 specular0;
	float3 p_ndc;

	ComputeDirectionalLight(L, pos, normal, view_vec, spec_exponent, diffuse0, specular0, p_ndc);

	const float shadow_factor = ShadowFactor(shadow_map, p_ndc);

	out_diffuse  = diffuse0  * shadow_factor;
	out_specular = specular0 * shadow_factor;
}



//----------------------------------------------------------------------------------
// ComputePointLight
//----------------------------------------------------------------------------------
// Computes the diffuse and specular terms in the lighting equation
// from a point light. We need to output the terms separately because
// later we will modify the individual terms.
//----------------------------------------------------------------------------------
void ComputePointLight(PointLight L,
                       float3 pos,
                       float3 normal,
                       float3 view_vec,
                       float  spec_exponent,
                       out float3 out_diffuse,
                       out float3 out_specular) {

	// Initialize outputs.
	out_diffuse = float3(0.0f, 0.0f, 0.0f);
	out_specular = float3(0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 light_vec = L.position - pos;

	// The distance from surface to light.
	float d = length(light_vec);

	// Range test.
	if (d > L.range)
		return;

	// Normalize the light vector.
	light_vec /= d;

	// Diffuse
	const float diffuse_factor = DiffuseFactor(light_vec, normal);
	out_diffuse = diffuse_factor * L.diffuse;

	// Specular
	const float specular_factor = SPECULAR_FACTOR_FUNC(light_vec, normal, view_vec, spec_exponent);
	out_specular = specular_factor * L.specular;

	// Attenution
	const float attenuation = Attenuation(d, L.attenuation);

	out_diffuse  *= attenuation;
	out_specular *= attenuation;
}


void ComputeShadowPointLight(ShadowPointLight L,
                             ShadowCubeMap shadow_map,
                             float3 pos,
                             float3 normal,
                             float3 view_vec,
                             float spec_exponent,
                             out float3 out_diffuse,
                             out float3 out_specular) {

	float3 diffuse0;
	float3 specular0;

	ComputePointLight(L.light, pos, normal, view_vec, spec_exponent, diffuse0, specular0);

	const float3 p_light       = mul(float4(pos, 1.0f), L.world_to_light).xyz;
	const float  shadow_factor = ShadowFactor(shadow_map, p_light, L.projection_values);

	out_diffuse  = diffuse0  * shadow_factor;
	out_specular = specular0 * shadow_factor;
}



//----------------------------------------------------------------------------------
// ComputeSpotLight
//----------------------------------------------------------------------------------
// Computes the diffuse and specular terms in the lighting equation
// from a spotlight. We need to output the terms separately because
// later we will modify the individual terms.
//----------------------------------------------------------------------------------
void ComputeSpotLight(SpotLight L,
                      float3 pos,
                      float3 normal,
                      float3 view_vec,
                      float  spec_exponent,
                      out float3 out_diffuse,
                      out float3 out_specular) {

	// Initialize outputs.
	out_diffuse = float3(0.0f, 0.0f, 0.0f);
	out_specular = float3(0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 light_vec = L.position - pos;

	// The distance from surface to light.
	float d = length(light_vec);

	// Range test.
	if (d > L.range)
		return;

	// Normalize the light vector.
	light_vec /= d;


	// Diffuse
	const float diffuse_factor = DiffuseFactor(light_vec, normal);
	out_diffuse = diffuse_factor * L.diffuse;

	// Specular
	const float specular_factor = SPECULAR_FACTOR_FUNC(light_vec, normal, view_vec, spec_exponent);
	out_specular = specular_factor * L.specular;

	// Attenution
	const float attenuation = Attenuation(d, L.attenuation);

	// Spot intensity
	const float intensity = SpotIntensity(light_vec, L.direction, L.cos_umbra, L.cos_penumbra);

	out_diffuse  *= attenuation * intensity;
	out_specular *= attenuation * intensity;
}


void ComputeShadowSpotLight(ShadowSpotLight L,
                            ShadowMap shadow_map,
                            float3 pos,
                            float3 normal,
                            float3 view_vec,
                            float spec_exponent,
                            out float3 out_diffuse,
                            out float3 out_specular) {
	
	float3 diffuse0;
	float3 specular0;

	ComputeSpotLight(L.light, pos, normal, view_vec, spec_exponent, diffuse0, specular0);

	const float4 p_proj        = mul(float4(pos, 1.0f), L.world_to_projection);
	const float3 p_ndc         = p_proj.xyz / p_proj.w;
	const float  shadow_factor = ShadowFactor(shadow_map, p_ndc);

	out_diffuse  = diffuse0  * shadow_factor;
	out_specular = specular0 * shadow_factor;
}


#endif //HLSL_LIGHT