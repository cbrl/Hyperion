#ifndef HLSL_LIGHTING
#define HLSL_LIGHTING

//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// DISABLE_SHADOW_MAPPING
// DISABLE_FOG


#include "include/lights.hlsli"
#include "include/brdf/brdf.hlsli"


namespace Lighting {

//----------------------------------------------------------------------------------
//  Constant Buffers
//----------------------------------------------------------------------------------

cbuffer LightBuffer : REG_B(SLOT_CBUFFER_LIGHT) {
	float3 g_ambient_intensity;
	float  lb_pad0;
};



namespace detail {

// Calculate the radiance contributed by a light
float3 CalculateLight(iLight light,
                      float3 p_world,
                      float3 n,
                      float3 p_to_view,
                      Material mat) {
	float3 radiance = 0.0f;

	float3 p_to_light, irradiance;
	light.Calculate(p_world, p_to_light, irradiance);

	if (any(irradiance)) {
		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);
		radiance = (D + S) * irradiance * saturate(dot(n, p_to_light));
	}

	return radiance;
}

// Calculate the radiance contributed by a shadow light
float3 CalculateLight(iShadowLight light,
                      iShadowMap shadow_map,
                      float3 p_world,
                      float3 n,
                      float3 p_to_view,
                      Material mat) {
	float3 radiance = 0.0f;

	float3 p_to_light, irradiance;
	light.Calculate(shadow_map, p_world, p_to_light, irradiance);

	if (any(irradiance)) {
		float3 D, S;
		BRDF::Compute(p_to_light, n, p_to_view, mat, D, S);
		radiance = (D + S)* irradiance* saturate(dot(n, p_to_light));
	}

	return radiance;
}

// Calculate the radiance for lights that don't cast shadows
float3 CalculateLights(float3 p_world,
                       float3 n,
                       float3 p_to_view,
                       Material mat) {

	float3 radiance = 0.0f;

	uint n_directional_lights = 0;
	uint n_point_lights = 0;
	uint n_spot_lights = 0;
	uint stride_unused;

	g_directional_lights.GetDimensions(n_directional_lights, stride_unused);
	g_point_lights.GetDimensions(n_point_lights, stride_unused);
	g_spot_lights.GetDimensions(n_spot_lights, stride_unused);

	// Directional Lights
	for (uint i0 = 0; i0 < n_directional_lights; ++i0) {
		radiance += CalculateLight(g_directional_lights[i0], p_world, n, p_to_view, mat);
	}

	// Point Lights
	for (uint i1 = 0; i1 < n_point_lights; ++i1) {
		radiance += CalculateLight(g_point_lights[i1], p_world, n, p_to_view, mat);
	}

	// Spot lights
	for (uint i2 = 0; i2 < n_spot_lights; ++i2) {
		radiance += CalculateLight(g_spot_lights[i2], p_world, n, p_to_view, mat);
	}

	return radiance;
}


// Calculate the radiance for lights that cast shadows
float3 CalculateShadowLights(float3 p_world,
                             float3 n,
                             float3 p_to_view,
                             Material mat) {

	float3 radiance = 0.0f;

	uint n_shadow_directional_lights = 0;
	uint n_shadow_point_lights = 0;
	uint n_shadow_spot_lights = 0;
	uint stride_unused;

	g_shadow_directional_lights.GetDimensions(n_shadow_directional_lights, stride_unused);
	g_shadow_point_lights.GetDimensions(n_shadow_point_lights, stride_unused);
	g_shadow_spot_lights.GetDimensions(n_shadow_spot_lights, stride_unused);

	// Directional Lights
	for (uint i0 = 0; i0 < n_shadow_directional_lights; ++i0) {
		#ifdef DISABLE_SHADOW_MAPPING
			radiance += CalculateLight(g_shadow_directional_lights[i0], p_world, n, p_to_view, mat);
		#else
			const ShadowMap shadow_map = {g_pcf_sampler, g_directional_light_smaps, i0};
			radiance += CalculateLight(g_shadow_directional_lights[i0], shadow_map, p_world, n, p_to_view, mat);
		#endif
	}

	// Point Lights
	for (uint i1 = 0; i1 < n_shadow_point_lights; ++i1) {
		#ifdef DISABLE_SHADOW_MAPPING
			radiance += CalculateLight(g_shadow_point_lights[i1], p_world, n, p_to_view, mat);
		#else
			const ShadowCubeMap shadow_map = {g_pcf_sampler, g_point_light_smaps, i1};
			radiance += CalculateLight(g_shadow_point_lights[i1], shadow_map, p_world, n, p_to_view, mat);
		#endif
	}

	// Spot Lights
	for (uint i2 = 0; i2 < n_shadow_spot_lights; ++i2) {
		#ifdef DISABLE_SHADOW_MAPPING
			radiance += CalculateLight(g_shadow_spot_lights[i2], p_world, n, p_to_view, mat);
		#else
			const ShadowMap shadow_map = {g_pcf_sampler, g_spot_light_smaps, i2};
			radiance += CalculateLight(g_shadow_spot_lights[i2], shadow_map, p_world, n, p_to_view, mat);
		#endif
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
	ambient *= g_ambient_intensity;

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