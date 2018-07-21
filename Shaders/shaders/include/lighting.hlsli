#ifndef HLSL_LIGHTING
#define HLSL_LIGHTING

//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// ENABLE_SHADOW_MAPPING

#include "include/light.hlsl"


void CalculateLights(float3 P, float3 N, float3 V, out float4 ambient, out float4 diffuse, out float4 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < num_directional_lights; ++i0) {
		float4 A, D, S;
		ComputeDirectionalLight(directional_lights[i0], mat, P, N, V,
								A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < num_point_lights; ++i1) {
		float4 A, D, S;
		ComputePointLight(point_lights[i1], mat, P, N, V,
						  A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}

	// Spot lights
	for (uint i2 = 0; i2 < num_spot_lights; ++i2) {
		float4 A, D, S;
		ComputeSpotLight(spot_lights[i2], mat, P, N, V,
						 A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}
}


void CalculateShadowedLights(float3 P, float3 N, float3 V, out float4 ambient, out float4 diffuse, out float4 specular) {

	// Directional Lights
	for (uint i0 = 0; i0 < num_shadow_directional_lights; ++i0) {
		float4 A, D, S;
		ShadowMap shadow_map = { pcf_sampler, directional_light_smaps, i0 };

		ComputeShadowedDirectionalLight(shadow_directional_lights[i0], shadow_map, mat, P, N, V,
		                                A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}

	// Point Lights
	for (uint i1 = 0; i1 < num_shadow_point_lights; ++i1) {
		float4 A, D, S;
		ShadowCubeMap cube_map = { pcf_sampler, point_light_smaps, i1 };

		ComputeShadowedPointLight(shadow_point_lights[i1], cube_map, mat, P, N, V,
		                          A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}

	// Spot Lights
	for (uint i2 = 0; i2 < num_shadow_spot_lights; ++i2) {
		float4 A, D, S;
		ShadowMap shadow_map = { pcf_sampler, spot_light_smaps, i2 };

		ComputeShadowedSpotLight(shadow_spot_lights[i2], shadow_map, mat, P, N, V,
		                         A, D, S);

		ambient  += A;
		diffuse  += D;
		specular += S;
	}
}


float4 CalculateLighting(float3 P, float3 N, float3 V, float4 tex_color, Material mat) {
	float4 lit_color = tex_color;

	// Start with a sum of zero.
	float4 ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	CalculateLights(P, N, V, ambient, diffuse, specular);

	#ifdef ENABLE_SHADOW_MAPPING
	CalculateShadowedLights(P, N, V, ambient, diffuse, specular);
	#endif


	// Calculate final color
	lit_color = tex_color * (ambient + diffuse) + specular;

	// Common to take alpha from diffuse mat and texture
	lit_color.a = mat.diffuse.a * tex_color.a;

	return lit_color;
}


#endif //HLSL_LIGHTING