#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"
#include "shaders\include\light.hlsl"


//----------------------------------------------------------------------------------
//  Constant Buffers
//----------------------------------------------------------------------------------

CONSTANT_BUFFER(Model, SLOT_CBUFFER_MODEL) {
	matrix world;
	matrix world_inv_transpose;
	matrix world_view_proj;
	matrix texTransform;
	Material mat;
};



//----------------------------------------------------------------------------------
//  SRVs
//----------------------------------------------------------------------------------

TEXTURE_2D(diffuse_map, SLOT_SRV_DIFFUSE);
// ambient, specular, etc...



float4 PS(PSInputPositionNormalTexture pin) : SV_Target {
	// The toEye vector is used in lighting.
	float3 toEye = camera_position - pin.positionW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;

	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);

	// Sample texture.
	texColor = diffuse_map.Sample(aniso_wrap, pin.tex);

	// Discard pixel if texture alpha < 0.1.  Note that we do this
	// test as soon as possible so that we can potentially exit the shader 
	// early, thereby skipping the rest of the shader code.
	clip(texColor.a - 0.1f);


	//----------------------------------------------------------------------------------
	// Lighting.
	//----------------------------------------------------------------------------------

	float4 litColor = texColor;

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (directional_light_count > 0) {
		// Sum the light contribution from each light source.  
		for (int i = 0; i < directional_light_count; ++i) {
			float4 A, D, S;
			ComputeDirectionalLight(mat, directional_lights[i], pin.normal, toEye,
									A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}
	}

	if (point_light_count > 0) {
		for (int i = 0; i < point_light_count; ++i) {
			float4 A, D, S;
			ComputePointLight(mat, point_lights[i], pin.positionW, pin.normal, toEye,
							  A, D, S);
			
			ambient += A;
			diffuse += D;
			spec    += S;
		}
	}

	if (spot_light_count > 0) {
		for (int i = 0; i < spot_light_count; ++i) {
			float4 A, D, S;
			ComputeSpotLight(mat, spot_lights[i], pin.positionW, pin.normal, toEye,
							 A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	// Modulate with late add.
	litColor = texColor * (ambient + diffuse) + spec;


	//----------------------------------------------------------------------------------
	// Fogging
	//----------------------------------------------------------------------------------

	float fogLerp = saturate((distToEye - fog_start) / fog_range);

	// Blend the fog color and the lit color.
	litColor = lerp(litColor, fog_color, fogLerp);


	// Common to take alpha from diffuse material and texture.
	litColor.a = mat.diffuse.a * texColor.a;

	litColor = saturate(litColor);

	return litColor;
}