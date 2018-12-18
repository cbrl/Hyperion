#ifndef HLSL_LIGHTING
#define HLSL_LIGHTING

//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// ENABLE_SHADOW_MAPPING

#include "include/light.hlsli"


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
void CalculateLights(float3 P,
                     float3 N,
                     float3 V,
                     float  spec_exponent,
                     inout float3 diffuse,
                     inout float3 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < g_num_directional_lights; ++i0) {
		float3 D, S;
		ComputeDirectionalLight(g_directional_lights[i0], P, N, V, spec_exponent,
								D, S);

		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < g_num_point_lights; ++i1) {
		float3 D, S;
		ComputePointLight(g_point_lights[i1], P, N, V, spec_exponent,
						  D, S);

		diffuse  += D;
		specular += S;
	}

	// Spot lights
	for (uint i2 = 0; i2 < g_num_spot_lights; ++i2) {
		float3 D, S;
		ComputeSpotLight(g_spot_lights[i2], P, N, V, spec_exponent,
						 D, S);

		diffuse  += D;
		specular += S;
	}
}


// Calculate the light intensities for shadow lights
void CalculateShadowLights(float3 P,
                           float3 N,
                           float3 V,
                           float  spec_exponent,
                           inout float3 diffuse,
                           inout float3 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < g_num_shadow_directional_lights; ++i0) {
		float3 D, S;
		const ShadowMap shadow_map = {g_pcf_sampler, g_directional_light_smaps, i0};

		ComputeShadowDirectionalLight(g_shadow_directional_lights[i0], shadow_map, P, N, V, spec_exponent,
		                              D, S);

		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < g_num_shadow_point_lights; ++i1) {
		float3 D, S;
		const ShadowCubeMap cube_map = {g_pcf_sampler, g_point_light_smaps, i1};

		ComputeShadowPointLight(g_shadow_point_lights[i1], cube_map, P, N, V, spec_exponent,
		                        D, S);

		diffuse  += D;
		specular += S;
	}

	// Spot Lights
	for (uint i2 = 0; i2 < g_num_shadow_spot_lights; ++i2) {
		float3 D, S;
		const ShadowMap shadow_map = { g_pcf_sampler, g_spot_light_smaps, i2 };

		ComputeShadowSpotLight(g_shadow_spot_lights[i2], shadow_map, P, N, V, spec_exponent,
		                       D, S);

		diffuse  += D;
		specular += S;
	}
}


// Calculate the light intensities for shadow lights with shadow mapping disabled
void CalculateShadowDisabledLights(float3 P,
                                   float3 N,
                                   float3 V,
                                   float spec_exponent,
                                   inout float3 diffuse,
                                   inout float3 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < g_num_shadow_directional_lights; ++i0) {
		float3 D, S;
		ComputeDirectionalLight(g_shadow_directional_lights[i0], P, N, V, spec_exponent,
		                        D, S);

		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < g_num_shadow_point_lights; ++i1) {
		float3 D, S;
		ComputePointLight(g_shadow_point_lights[i1].light, P, N, V, spec_exponent,
		                  D, S);

		diffuse  += D;
		specular += S;
	}

	// Spot Lights
	for (uint i2 = 0; i2 < g_num_shadow_spot_lights; ++i2) {
		float3 D, S;
		ComputeSpotLight(g_shadow_spot_lights[i2].light, P, N, V, spec_exponent,
		                 D, S);

		diffuse  += D;
		specular += S;
	}
}


float4 CalculateLighting(float3 P, float3 N, float3 V, float4 base_color, Material material) {

	// Diffuse and specular light intensity
	float3 l_diffuse   = { 0.0f, 0.0f, 0.0f };
	float3 l_specular  = { 0.0f, 0.0f, 0.0f };
	
	CalculateLights(P, N, V, material.spec_exponent, l_diffuse, l_specular);

	#ifdef ENABLE_SHADOW_MAPPING
	CalculateShadowLights(P, N, V, material.spec_exponent, l_diffuse, l_specular);
	#else
	CalculateShadowDisabledLights(P, N, V, material.spec_exponent, l_diffuse, l_specular);
	#endif


	// Calculate final color
	float3 out_color = (g_ambient_intensity.xyz * material.ambient.xyz + l_diffuse) * base_color.xyz
	                   + (l_specular * material.spec_scale * material.specular.xyz);

	return float4(out_color, base_color.a);
}


#endif //HLSL_LIGHTING