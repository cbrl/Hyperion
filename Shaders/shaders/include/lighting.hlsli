#ifndef HLSL_LIGHTING
#define HLSL_LIGHTING

//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// ENABLE_SHADOW_MAPPING

#include "include/light.hlsli"
#include "include/brdf.hlsli"


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



// Calculate the light intensities for lights that don't cast shadows
void CalculateLights(float3 p_world,
                     float3 n,
                     float3 p_to_v,
                     Material mat,
                     inout float3 diffuse,
                     inout float3 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < g_num_directional_lights; ++i0) {
		float3 p_to_l, E_factor;
		g_directional_lights[i0].Calculate(p_world, p_to_l, E_factor);

		float3 D, S;
		ComputeBRDF(p_to_l, n, p_to_v, mat, D, S);

		diffuse  += D * E_factor;
		specular += S * E_factor;
	}

	// Point Lights
	for (uint i1 = 0; i1 < g_num_point_lights; ++i1) {
		float3 p_to_l, E_factor;
		g_point_lights[i1].Calculate(p_world, p_to_l, E_factor);

		float3 D, S;
		ComputeBRDF(p_to_l, n, p_to_v, mat, D, S);

		diffuse  += D * E_factor;
		specular += S * E_factor;
	}

	// Spot lights
	for (uint i2 = 0; i2 < g_num_spot_lights; ++i2) {
		float3 p_to_l, E_factor;
		g_spot_lights[i2].Calculate(p_world, p_to_l, E_factor);

		float3 D, S;
		ComputeBRDF(p_to_l, n, p_to_v, mat, D, S);

		diffuse  += D * E_factor;
		specular += S * E_factor;
	}
}


// Calculate the light intensities for shadow lights
void CalculateShadowLights(float3 p_world,
                           float3 n,
                           float3 p_to_v,
                           Material mat,
                           inout float3 diffuse,
                           inout float3 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < g_num_shadow_directional_lights; ++i0) {
		float3 p_to_l, E_factor;

		#ifdef ENABLE_SHADOW_MAPPING
		const ShadowMap shadow_map = {g_pcf_sampler, g_directional_light_smaps, i0};
		g_shadow_directional_lights[i0].Calculate(shadow_map, p_world, p_to_l, E_factor);
		#else
		ComputeDirectionalLight(g_shadow_directional_lights[i0], p_world, p_to_l, E_factor);
		g_shadow_directional_lights[i0].Calculate(p_world, p_to_l, E_factor);
		#endif

		float3 D, S;
		ComputeBRDF(p_to_l, n, p_to_v, mat, D, S);

		diffuse  += D * E_factor;
		specular += S * E_factor;
	}

	// Point Lights
	for (uint i1 = 0; i1 < g_num_shadow_point_lights; ++i1) {
		float3 p_to_l, E_factor;

		#ifdef ENABLE_SHADOW_MAPPING
		const ShadowCubeMap shadow_map = {g_pcf_sampler, g_point_light_smaps, i1};
		g_shadow_point_lights[i1].Calculate(shadow_map, p_world, p_to_l, E_factor);
		#else
		ComputePointLight(g_shadow_point_lights[i1].light, p_world, p_to_l, E_factor);
		g_shadow_point_lights[i1].Calculate(p_world, p_to_l, E_factor);
		#endif

		float3 D, S;
		ComputeBRDF(p_to_l, n, p_to_v, mat, D, S);

		diffuse  += D * E_factor;
		specular += S * E_factor;
	}

	// Spot Lights
	for (uint i2 = 0; i2 < g_num_shadow_spot_lights; ++i2) {
		float3 p_to_l, E_factor;

		#ifdef ENABLE_SHADOW_MAPPING
		const ShadowMap shadow_map = {g_pcf_sampler, g_spot_light_smaps, i2};
		g_shadow_spot_lights[i2].Calculate(shadow_map, p_world, p_to_l, E_factor);
		#else
		ComputeSpotLight(g_shadow_spot_lights[i2].light, p_world, p_to_l, E_factor);
		g_shadow_spot_lights[i2].Calculate(p_world, p_to_l, E_factor);
		#endif

		float3 D, S;
		ComputeBRDF(p_to_l, n, p_to_v, mat, D, S);

		diffuse  += D * E_factor;
		specular += S * E_factor;
	}
}


float3 CalculateLighting(float3 p_world, float3 n, float3 p_to_v, Material material) {

	// Diffuse and specular light intensity
	float3 l_diffuse   = { 0.0f, 0.0f, 0.0f };
	float3 l_specular  = { 0.0f, 0.0f, 0.0f };
	
	CalculateLights(p_world, n, p_to_v, material, l_diffuse, l_specular);
	CalculateShadowLights(p_world, n, p_to_v, material, l_diffuse, l_specular);

	// Calculate final color
	const float3 ambient     = g_ambient_intensity.xyz * material.base_color.xyz;
	const float3 final_color = ambient + l_diffuse + l_specular;

	return final_color;
}


#endif //HLSL_LIGHTING