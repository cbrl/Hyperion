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

	float4 g_ambient_color;
};



void CalculateLights(float3 P,
                     float3 N,
                     float3 V,
                     Material material,
                     out float4 diffuse,
                     out float4 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < g_num_directional_lights; ++i0) {
		float4 D, S;
		ComputeDirectionalLight(g_directional_lights[i0], material, P, N, V,
								D, S);

		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < g_num_point_lights; ++i1) {
		float4 D, S;
		ComputePointLight(g_point_lights[i1], material, P, N, V,
						  D, S);

		diffuse  += D;
		specular += S;
	}

	// Spot lights
	for (uint i2 = 0; i2 < g_num_spot_lights; ++i2) {
		float4 D, S;
		ComputeSpotLight(g_spot_lights[i2], material, P, N, V,
						 D, S);

		diffuse  += D;
		specular += S;
	}
}


void CalculateShadowedLights(float3 P,
                             float3 N,
                             float3 V,
                             Material material,
                             out float4 diffuse,
                             out float4 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < g_num_shadow_directional_lights; ++i0) {
		float4 D, S;
		ShadowMap shadow_map = { g_pcf_sampler, g_directional_light_smaps, i0 };

		ComputeShadowedDirectionalLight(g_shadow_directional_lights[i0], shadow_map, material, P, N, V,
		                                D, S);

		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < g_num_shadow_point_lights; ++i1) {
		float4 D, S;
		ShadowCubeMap cube_map = { g_pcf_sampler, g_point_light_smaps, i1 };

		ComputeShadowedPointLight(g_shadow_point_lights[i1], cube_map, material, P, N, V,
		                          D, S);

		diffuse  += D;
		specular += S;
	}

	// Spot Lights
	for (uint i2 = 0; i2 < g_num_shadow_spot_lights; ++i2) {
		float4 D, S;
		ShadowMap shadow_map = { g_pcf_sampler, g_spot_light_smaps, i2 };

		ComputeShadowedSpotLight(g_shadow_spot_lights[i2], shadow_map, material, P, N, V,
		                         D, S);

		diffuse  += D;
		specular += S;
	}
}


float4 CalculateLighting(float3 P, float3 N, float3 V, float4 tex_color, Material material) {

	float4 diffuse   = { 0.0f, 0.0f, 0.0f, 1.0f };
	float4 specular  = { 0.0f, 0.0f, 0.0f, 1.0f };

	CalculateLights(P, N, V, material, diffuse, specular);

	#ifdef ENABLE_SHADOW_MAPPING
	CalculateShadowedLights(P, N, V, material, diffuse, specular);
	#endif


	// Calculate final color
	float4 out_color = tex_color * (g_ambient_color + diffuse) + specular;

	// Common to take alpha from diffuse mat and texture
	out_color.a = material.diffuse.a * tex_color.a;

	return out_color;
}


#endif //HLSL_LIGHTING