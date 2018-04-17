#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"
#include "shaders\include\light.hlsl"
#include "shaders\include\normal.hlsl"


//----------------------------------------------------------------------------------
//  Constant Buffers
//----------------------------------------------------------------------------------

CONSTANT_BUFFER(Model, SLOT_CBUFFER_MODEL) {
	matrix   world;
	matrix   world_inv_transpose;
	matrix   world_view_proj;
	matrix   texTransform;
	Material mat;
};



//----------------------------------------------------------------------------------
//  SRVs
//----------------------------------------------------------------------------------

TEXTURE_CUBE(env_map, SLOT_SRV_SKYBOX);

TEXTURE_2D(diffuse_map, SLOT_SRV_DIFFUSE);
TEXTURE_2D(normal_map,  SLOT_SRV_NORMAL);
// ambient, specular, etc...



float4 PS(PSPositionNormalTexture pin) : SV_Target {
	// The to_eye vector is used in lighting
	float3 to_eye = camera_position - pin.w_position;

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

	if (directional_light_count > 0) {
		// Sum the light contribution from each light source
		for (int i = 0; i < directional_light_count; ++i) {
			float4 A, D, S;
			ComputeDirectionalLight(mat, directional_lights[i], transformed_normal, to_eye,
									A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

		lit_color = tex_color * (ambient + diffuse) + spec;

		if (mat.reflection_enabled) {
			float3 incident = -to_eye;
			float3 reflection_vec = reflect(incident, pin.normal);
			float4 reflection_color = env_map.Sample(aniso_wrap, reflection_vec);

			lit_color += mat.reflect * reflection_color;
		}
	}

	if (point_light_count > 0) {
		for (int i = 0; i < point_light_count; ++i) {
			float4 A, D, S;
			ComputePointLight(mat, point_lights[i], pin.w_position, transformed_normal, to_eye,
							  A, D, S);
			
			ambient += A;
			diffuse += D;
			spec    += S;
		}

		lit_color = tex_color * (ambient + diffuse) + spec;

		if (mat.reflection_enabled) {
			float3 incident = -to_eye;
			float3 reflection_vec = reflect(incident, pin.normal);
			float4 reflection_color = env_map.Sample(aniso_wrap, reflection_vec);

			lit_color += mat.reflect * reflection_color;
		}
	}

	if (spot_light_count > 0) {
		for (int i = 0; i < spot_light_count; ++i) {
			float4 A, D, S;
			ComputeSpotLight(mat, spot_lights[i], pin.w_position, transformed_normal, to_eye,
							 A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		lit_color = tex_color * (ambient + diffuse) + spec;

		if (mat.reflection_enabled) {
			float3 incident = -to_eye;
			float3 reflection_vec = reflect(incident, pin.normal);
			float4 reflection_color = env_map.Sample(aniso_wrap, reflection_vec);

			lit_color += mat.reflect * reflection_color;
		}
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