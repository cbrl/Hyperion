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
	uint num_directional_lights;
	uint num_point_lights;
	uint num_spot_lights;
	uint lb_pad0;

	// Shadowed Lights
	uint num_shadow_directional_lights;
	uint num_shadow_point_lights;
	uint num_shadow_spot_lights;
	uint lb_pad1;

	float4 ambient_color;
};



float4 CalculateLights(float3 P, float3 N, float3 V, Material material, float4 tex_color) {

	float4 diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Directional Lights
	for (uint i0 = 0; i0 < num_directional_lights; ++i0) {
		float4 D, S;
		ComputeDirectionalLight(directional_lights[i0], material, P, N, V,
								D, S);

		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < num_point_lights; ++i1) {
		float4 D, S;
		ComputePointLight(point_lights[i1], material, P, N, V,
						  D, S);

		diffuse  += D;
		specular += S;
	}

	// Spot lights
	for (uint i2 = 0; i2 < num_spot_lights; ++i2) {
		float4 D, S;
		ComputeSpotLight(spot_lights[i2], material, P, N, V,
						 D, S);

		diffuse  += D;
		specular += S;
	}

	// Calculate final color
	float4 out_color = tex_color * (ambient_color + diffuse) + specular;

	// Common to take alpha from diffuse mat and texture
	out_color.a = material.diffuse.a * tex_color.a;

	return out_color;
}


float4 CalculateShadowedLights(float3 P, float3 N, float3 V, Material material, float4 tex_color) {

	float4 diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Directional Lights
	for (uint i0 = 0; i0 < num_shadow_directional_lights; ++i0) {
		float4 D, S;
		ShadowMap shadow_map = { pcf_sampler, directional_light_smaps, i0 };

		ComputeShadowedDirectionalLight(shadow_directional_lights[i0], shadow_map, material, P, N, V,
		                                D, S);

		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < num_shadow_point_lights; ++i1) {
		float4 D, S;
		ShadowCubeMap cube_map = { pcf_sampler, point_light_smaps, i1 };

		ComputeShadowedPointLight(shadow_point_lights[i1], cube_map, material, P, N, V,
		                          D, S);

		diffuse  += D;
		specular += S;
	}

	// Spot Lights
	for (uint i2 = 0; i2 < num_shadow_spot_lights; ++i2) {
		float4 D, S;
		ShadowMap shadow_map = { pcf_sampler, spot_light_smaps, i2 };

		ComputeShadowedSpotLight(shadow_spot_lights[i2], shadow_map, material, P, N, V,
		                         D, S);

		diffuse  += D;
		specular += S;
	}

	// Calculate final color
	float4 out_color = tex_color * (ambient_color + diffuse) + specular;

	// Common to take alpha from diffuse mat and texture
	out_color.a = material.diffuse.a * tex_color.a;

	return out_color;
}


float4 CalculateLighting(float3 P, float3 N, float3 V, float4 tex_color, Material material) {

	float4 lit_color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	lit_color += CalculateLights(P, N, V, material, tex_color);

	#ifdef ENABLE_SHADOW_MAPPING
	lit_color += CalculateShadowedLights(P, N, V, material, tex_color);
	#endif

	return lit_color;
}


#endif //HLSL_LIGHTING