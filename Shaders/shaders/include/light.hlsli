#ifndef HLSL_LIGHT
#define HLSL_LIGHT

#include "include/global.hlsli"
#include "include/material.hlsli"
#include "include/math.hlsli"


//----------------------------------------------------------------------------------
// Attenuation
//----------------------------------------------------------------------------------
//          d: distance to light
// att_values: constant, linear, and quadratic attenuation values respectively
//----------------------------------------------------------------------------------
float Attenuation(float d, float3 att_values) {
	const float denom = dot(att_values, float3(1.0f, d, sqr(d)));
	return 1.0f / max(denom, 0.01f);
}

float DistanceAttenuation(float d) {
	return Attenuation(d, float3(0.0f, 0.0f, 1.0f));
}


//----------------------------------------------------------------------------------
// SpotIntensity
//----------------------------------------------------------------------------------
//         L: surface-to-light vector
//         D: light direction
// cos_theta: cosine of the umbra angle
//   cos_phi: cosine of the penumbra angle
//----------------------------------------------------------------------------------
float SpotIntensity(float3 L, float3 D, float cos_theta, float cos_phi) {
	const float alpha = dot(-L, D);
	return smoothstep(cos_phi, cos_theta, alpha);
}




//----------------------------------------------------------------------------------
// Shadow Map Structs
//----------------------------------------------------------------------------------

struct ShadowMap {
	SamplerComparisonState sam_shadow;
	Texture2DArray maps;
	uint index;

	float ShadowFactor(float3 p_ndc) {
		const float2 uv = float2(0.5f, -0.5f) * p_ndc.xy + 0.5f;
		const float3 location = {uv, index};

		return maps.SampleCmpLevelZero(sam_shadow, location, p_ndc.z);
	}
};


struct ShadowCubeMap {
	SamplerComparisonState sam_shadow;
	TextureCubeArray maps;
	uint index;

	float ShadowFactor(float3 p_light, float2 projection_values) {
		const float3 p_light_abs = abs(p_light);

		const float p_light_z = max(p_light_abs.x, max(p_light_abs.y, p_light_abs.z));
		const float p_ndc_z = projection_values.x + projection_values.y / p_light_z;
		const float4 location = float4(p_light, index);

		return maps.SampleCmpLevelZero(sam_shadow, location, p_ndc_z);
	}
};




//----------------------------------------------------------------------------------
// Directional Light
//----------------------------------------------------------------------------------

struct DirectionalLight {
	float3 intensity;
	float  pad0;
	float3 direction;
	float  pad1;
	matrix world_to_projection;

	void Calculate(float3 p_world, out float3 p_to_light, out float3 irradiance, out float3 p_ndc) {
		// The light vector aims opposite the direction the light rays travel
		p_to_light = -direction;

		const float4 p_proj = mul(float4(p_world, 1.0f), world_to_projection);
		p_ndc = Homogenize(p_proj);

		irradiance = (any(1.0f < abs(p_ndc)) || 0.0f > p_ndc.z) ? 0.0f : intensity;
	}

	void Calculate(float3 p_world, out float3 p_to_light, out float3 irradiance) {
		float3 p_ndc;
		float3 p_to_l0;
		float3 irradiance0;

		Calculate(p_world, p_to_l0, irradiance0, p_ndc);

		p_to_light   = p_to_l0;
		irradiance = irradiance0;
	}

	void Calculate(ShadowMap shadow_map, float3 p_world, out float3 p_to_light, out float3 irradiance) {
		float3 p_ndc;
		float3 p_to_l0;
		float3 irradiance0;

		Calculate(p_world, p_to_l0, irradiance0, p_ndc);

		p_to_light = p_to_l0;

		const float shadow_factor = shadow_map.ShadowFactor(p_ndc);
		irradiance = irradiance0 * shadow_factor;
	}
};




//----------------------------------------------------------------------------------
// Point Light
//----------------------------------------------------------------------------------

struct PointLight {
	float3 intensity;
	float  pad0;
	float3 position;
	float  range;
	float3 attenuation;
	float  pad1;

	void Calculate(float3 p_world, out float3 p_to_light, out float3 irradiance) {
		// The vector from the surface to the light
		p_to_light = position - p_world;

		// The distance from surface to light
		const float d = length(p_to_light);

		// Normalize the light vector
		p_to_light /= d;

		// Attenuation
		const float att_factor = Attenuation(d, attenuation);
		irradiance = intensity * att_factor;
	}
};


struct ShadowPointLight : PointLight {
	matrix world_to_light;
	float2 projection_values;
	float2 pad1;

	void Calculate(float3 p_world, out float3 p_to_light, out float3 irradiance) {
		PointLight::Calculate(p_world, p_to_light, irradiance);
	}

	void Calculate(ShadowCubeMap shadow_map, float3 p_world, out float3 p_to_light, out float3 irradiance) {
		float3 p_to_l0;
		float3 irradiance0;

		PointLight::Calculate(p_world, p_to_l0, irradiance0);

		p_to_light = p_to_l0;

		const float3 p_light       = mul(float4(p_world, 1.0f), world_to_light).xyz;
		const float  shadow_factor = shadow_map.ShadowFactor(p_light, projection_values);

		irradiance = irradiance0 * shadow_factor;
	}
};




//----------------------------------------------------------------------------------
// Spot Light
//----------------------------------------------------------------------------------

struct SpotLight {
	float3 intensity;
	float  pad;
	float3 position;
	float  range;
	float3 direction;
	float  cos_umbra;
	float  cos_penumbra;
	float3 attenuation;

	void Calculate(float3 p_world, out float3 p_to_light, out float3 irradiance) {
		// The vector from the surface to the light.
		p_to_light = position - p_world;

		// The distance from surface to light.
		float d = length(p_to_light);

		// Normalize the light vector.
		p_to_light /= d;

		const float att_factor  = Attenuation(d, attenuation);
		const float spot_factor = SpotIntensity(p_to_light, direction, cos_umbra, cos_penumbra);

		irradiance = intensity * att_factor * spot_factor;
	}
};

struct ShadowSpotLight : SpotLight {
	matrix world_to_projection;

	void Calculate(float3 p_world, out float3 p_to_light, out float3 irradiance) {
		SpotLight::Calculate(p_world, p_to_light, irradiance);
	}

	void Calculate(ShadowMap shadow_map, float3 p_world, out float3 p_to_light, out float3 irradiance) {
		float3 p_to_l0;
		float3 irradiance0;

		SpotLight::Calculate(p_world, p_to_l0, irradiance0);

		p_to_light = p_to_l0;

		const float4 p_proj        = mul(float4(p_world, 1.0f), world_to_projection);
		const float3 p_ndc         = Homogenize(p_proj);
		const float  shadow_factor = shadow_map.ShadowFactor(p_ndc);

		irradiance = irradiance0 * shadow_factor;
	}
};




//----------------------------------------------------------------------------------
//  Structured Buffers
//----------------------------------------------------------------------------------

// Lights
StructuredBuffer<DirectionalLight> g_directional_lights : REG_T(SLOT_SRV_DIRECTIONAL_LIGHTS);
StructuredBuffer<PointLight> g_point_lights : REG_T(SLOT_SRV_POINT_LIGHTS);
StructuredBuffer<SpotLight> g_spot_lights : REG_T(SLOT_SRV_SPOT_LIGHTS);


// Shadow Lights
StructuredBuffer<DirectionalLight> g_shadow_directional_lights : REG_T(SLOT_SRV_DIRECTIONAL_LIGHTS_SHADOW);
Texture2DArray g_directional_light_smaps : REG_T(SLOT_SRV_DIRECTIONAL_LIGHT_SHADOW_MAPS);

StructuredBuffer<ShadowPointLight> g_shadow_point_lights : REG_T(SLOT_SRV_POINT_LIGHTS_SHADOW);
TextureCubeArray g_point_light_smaps : REG_T(SLOT_SRV_POINT_LIGHT_SHADOW_MAPS);

StructuredBuffer<ShadowSpotLight> g_shadow_spot_lights : REG_T(SLOT_SRV_SPOT_LIGHTS_SHADOW);
Texture2DArray g_spot_light_smaps : REG_T(SLOT_SRV_SPOT_LIGHT_SHADOW_MAPS);



#endif //HLSL_LIGHT