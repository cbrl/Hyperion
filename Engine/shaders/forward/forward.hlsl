#include "shaders\forward\forward_include.hlsl"
#include "shaders\include\light.hlsl"


//----------------------------------------------------------------------------------
//  SRVs
//----------------------------------------------------------------------------------

TEXTURE_CUBE(env_map, SLOT_SRV_SKYBOX);

TEXTURE_2D(diffuse_map, SLOT_SRV_DIFFUSE);
TEXTURE_2D(normal_map,  SLOT_SRV_NORMAL);
// ambient, specular, etc...



float4 PS(PSPositionNormalTexture pin) : SV_Target {
	// The to_eye vector is used in lighting
	float3 to_eye = camera_position - pin.position_world;

	// Cache the distance to the eye from this surface point
	float dist_to_eye = length(to_eye);

	// Normalize
	to_eye /= dist_to_eye;

	// Sample texture
	float4 tex_color;
	if (mat.has_texture) {
		tex_color = diffuse_map.Sample(aniso_wrap, pin.tex);
	}
	else {
		tex_color = mat.diffuse;
	}

	// Discard pixel if texture alpha < 0.1.  Note that we do this
	// test as soon as possible so that we can potentially exit the shader 
	// early, thereby skipping the rest of the shader code.
	clip(tex_color.a - 0.1f);


	//----------------------------------------------------------------------------------
	// Normal Mapping
	//----------------------------------------------------------------------------------

	float3 normal_map_sample = normal_map.Sample(linear_wrap, pin.tex);
	float3 transformed_normal;

	if (normal_map_sample.z) {
		transformed_normal = TransformNormal(pin.position, pin.normal, pin.tex, normal_map_sample);
	}
	else {
		transformed_normal = pin.normal;
	}



	//----------------------------------------------------------------------------------
	// Lighting
	//----------------------------------------------------------------------------------

	float4 lit_color = tex_color;

	// Start with a sum of zero.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (num_directional_lights > 0) {
		// Sum the light contribution from each light source
		for (uint i = 0; i < num_directional_lights; ++i) {
			float4 A, D, S;
			ComputeDirectionalLight(directional_lights[i], mat, transformed_normal, to_eye,
									A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	if (num_shadow_directional_lights > 0) {
		for (uint i = 0; i < num_shadow_directional_lights; ++i) {
			float4 A, D, S;
			ShadowMap shadow_map = { pcf_sampler, directional_light_smaps, i };

			ComputeShadowedDirectionalLight(shadow_directional_lights[i], shadow_map, mat, pin.position_world, transformed_normal, to_eye,
											A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	if (num_point_lights > 0) {
		for (uint i = 0; i < num_point_lights; ++i) {
			float4 A, D, S;
			ComputePointLight(point_lights[i], mat, pin.position_world, transformed_normal, to_eye,
							  A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	if (num_shadow_point_lights > 0) {
		for (uint i = 0; i < num_shadow_point_lights; ++i) {
			float4 A, D, S;
			ShadowCubeMap cube_map = { pcf_sampler, point_light_smaps, i };

			ComputeShadowedPointLight(shadow_point_lights[i], cube_map, mat, pin.position_world, transformed_normal, to_eye,
									  A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	if (num_spot_lights > 0) {
		for (uint i = 0; i < num_spot_lights; ++i) {
			float4 A, D, S;
			ComputeSpotLight(spot_lights[i], mat, pin.position_world, transformed_normal, to_eye,
							 A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	if (num_shadow_spot_lights > 0) {
		for (uint i = 0; i < num_shadow_spot_lights; ++i) {
			float4 A, D, S;
			ShadowMap shadow_map = { pcf_sampler, spot_light_smaps, i };

			ComputeShadowedSpotLight(shadow_spot_lights[i], shadow_map, mat, pin.position_world, transformed_normal, to_eye,
									 A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}


	lit_color = tex_color * (ambient + diffuse) + spec;

	if (mat.reflection_enabled) {
		float3 incident = -to_eye;
		float3 reflection_vec = reflect(incident, pin.normal);
		float4 reflection_color = env_map.Sample(aniso_wrap, reflection_vec);

		lit_color += mat.reflect * reflection_color;
	}


	//----------------------------------------------------------------------------------
	// Fogging
	//----------------------------------------------------------------------------------

	float fogLerp = saturate((dist_to_eye - fog_start) / fog_range);

	// Blend the fog color and the lit color
	lit_color = lerp(lit_color, fog_color, fogLerp);


	// Common to take alpha from diffuse mat and texture
	lit_color.a = mat.diffuse.a * tex_color.a;

	lit_color = saturate(lit_color);

	return lit_color;
}