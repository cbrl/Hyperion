#ifndef HLSL_LIGHTING
#define HLSL_LIGHTING

//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// DISABLE_SHADOW_MAPPING
// DISABLE_FOG


#include "include/light.hlsli"
#include "include/brdf/brdf.hlsli"


namespace Lighting {

//----------------------------------------------------------------------------------
//  Constant Buffers
//----------------------------------------------------------------------------------

cbuffer LightBuffer : REG_B(SLOT_CBUFFER_LIGHT) {
	// Lights
	uint g_num_directional_lights;
	uint g_num_point_lights;
	uint g_num_spot_lights;
	uint lb_pad0;

	// Shadowed Lights
	uint g_num_shadow_directional_lights;
	uint g_num_shadow_point_lights;
	uint g_num_shadow_spot_lights;
	uint lb_pad1;

	float4 g_ambient_intensity;
};



namespace detail {

// Calculate the radiance for lights that don't cast shadows
float3 CalculateLights(float3 p_world,
                       float3 n,
                       float3 p_to_view,
                       Material mat) {

	float3 radiance = 0.0f;

	//----------------------------------------------------------------------------------
	// Directional Lights
	//----------------------------------------------------------------------------------
	for (uint i0 = 0; i0 < g_num_directional_lights; ++i0) {
		float3 p_to_light, irradiance;
		g_directional_lights[i0].Calculate(p_world, p_to_light, irradiance);

		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);

		radiance += (D + S) * irradiance * saturate(dot(n, p_to_light));
	}

	//----------------------------------------------------------------------------------
	// Point Lights
	//----------------------------------------------------------------------------------
	for (uint i1 = 0; i1 < g_num_point_lights; ++i1) {
		float3 p_to_light, irradiance;
		g_point_lights[i1].Calculate(p_world, p_to_light, irradiance);

		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);

		radiance += (D + S) * irradiance * saturate(dot(n, p_to_light));
	}

	//----------------------------------------------------------------------------------
	// Spot lights
	//----------------------------------------------------------------------------------
	for (uint i2 = 0; i2 < g_num_spot_lights; ++i2) {
		float3 p_to_light, irradiance;
		g_spot_lights[i2].Calculate(p_world, p_to_light, irradiance);

		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);

		radiance += (D + S) * irradiance * saturate(dot(n, p_to_light));
	}

	return radiance;
}


// Calculate the radiance for lights that cast shadows
float3 CalculateShadowLights(float3 p_world,
                             float3 n,
                             float3 p_to_view,
                             Material mat) {

	float3 radiance = 0.0f;

	//----------------------------------------------------------------------------------
	// Directional Lights
	//----------------------------------------------------------------------------------
	for (uint i0 = 0; i0 < g_num_shadow_directional_lights; ++i0) {
		float3 p_to_light, irradiance;

		#ifdef DISABLE_SHADOW_MAPPING
		g_shadow_directional_lights[i0].Calculate(p_world, p_to_light, irradiance);
		#else
		const ShadowMap shadow_map = {g_pcf_sampler, g_directional_light_smaps, i0};
		g_shadow_directional_lights[i0].Calculate(shadow_map, p_world, p_to_light, irradiance);
		#endif

		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);

		radiance += (D + S) * irradiance * saturate(dot(n, p_to_light));
	}

	//----------------------------------------------------------------------------------
	// Point Lights
	//----------------------------------------------------------------------------------
	for (uint i1 = 0; i1 < g_num_shadow_point_lights; ++i1) {
		float3 p_to_light, irradiance;

		#ifdef DISABLE_SHADOW_MAPPING
		g_shadow_point_lights[i1].Calculate(p_world, p_to_light, irradiance);
		#else
		const ShadowCubeMap shadow_map = {g_pcf_sampler, g_point_light_smaps, i1};
		g_shadow_point_lights[i1].Calculate(shadow_map, p_world, p_to_light, irradiance);
		#endif

		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);

		radiance += (D + S) * irradiance * saturate(dot(n, p_to_light));
	}

	//----------------------------------------------------------------------------------
	// Spot Lights
	//----------------------------------------------------------------------------------
	for (uint i2 = 0; i2 < g_num_shadow_spot_lights; ++i2) {
		float3 p_to_light, irradiance;

		#ifdef DISABLE_SHADOW_MAPPING
		g_shadow_spot_lights[i2].Calculate(p_world, p_to_light, irradiance);
		#else
		const ShadowMap shadow_map = {g_pcf_sampler, g_spot_light_smaps, i2};
		g_shadow_spot_lights[i2].Calculate(shadow_map, p_world, p_to_light, irradiance);
		#endif

		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);

		radiance += (D + S) * irradiance * saturate(dot(n, p_to_light));
	}

	return radiance;
}

} //namespace detail



float3 CalculateLighting(float3 p_world, float3 n, Material material) {

	float3 radiance = 0.0f;

	float3      p_to_view      = CameraPosition() - p_world;
	const float dist_p_to_view = length(p_to_view);

	p_to_view /= dist_p_to_view;
	
	// Calculate radiance
	radiance += detail::CalculateLights(p_world, n, p_to_view, material);
	radiance += detail::CalculateShadowLights(p_world, n, p_to_view, material);

	// Calculate ambient light
	float3 ambient;
	float3 null;
	BRDF::Lambert(0.0f, 0.0f, 0.0f, material, ambient, null);
	ambient *= g_ambient_intensity.xyz;

	// Calculate fog
	#ifndef DISABLE_FOG
	const float fog = exp(-dist_p_to_view * g_fog.density);
	radiance = lerp(g_fog.color, radiance, fog);
	#endif

	// Calculate final color
	return radiance + ambient + material.emissive;
}

} //namespace Lighting



#endif //HLSL_LIGHTING