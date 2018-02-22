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
	if (directional_light_count > 0) {
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		//[unroll]
		for (int i = 0; i < directional_light_count; ++i) {
			float4 A, D, S;
			ComputeDirectionalLight(mat, directional_lights[i], pin.normal, toEye,
									A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		// Modulate with late add.
		litColor = texColor * (ambient + diffuse) + spec;
	}

	//----------------------------------------------------------------------------------
	// Fogging
	//----------------------------------------------------------------------------------

	//float fogLerp = saturate((distToEye - fog_start) / fog_range);

	// Blend the fog color and the lit color.
	//litColor = lerp(litColor, fog_color, fogLerp);


	// Common to take alpha from diffuse material and texture.
	litColor.a = mat.diffuse.a * texColor.a;

	return litColor;

















	//float4 textureColor;
	//float3 lightDir;
	//float lightIntensity;
	//float4 color;
	//float3 reflection;
	//float4 specular;


	//// The toEye vector is used in lighting.
	//float3 toEye = camera_position - pin.positionW;

	//// Cache the distance to the eye from this surface point.
	//float distToEye = length(toEye);

	//// Normalize.
	////toEye /= distToEye;
	//toEye = normalize(toEye);


	//// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	//textureColor = diffuse_map.Sample(linear_wrap, pin.tex);

	//// Set the default output color to the ambient light value
	//color = directional_lights[0].ambient;

	//// Initialize the specular color.
	//specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//// Invert the light direction for calculations.
	//lightDir = -directional_lights[0].direction;

	//// Calculate the amount of light on this pixel.
	//lightIntensity = saturate(dot(pin.normal, lightDir));

	//if (lightIntensity > 0.0f) {
	//	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	//	color += (directional_lights[0].diffuse * lightIntensity);

	//	// Saturate the ambient and diffuse color.
	//	color = saturate(color);

	//	// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
	//	reflection = normalize(2 * lightIntensity * pin.normal - lightDir);

	//	// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
	//	specular = pow(saturate(dot(reflection, toEye)), directional_lights[0].specular.w);
	//}

	//// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	//color = color * textureColor;

	//// Add the specular component last to the output color.
	//color = saturate(color + specular);

	//return color;
}