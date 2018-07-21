//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// ENABLE_LIGHTING

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------
#define ENABLE_SHADOW_MAPPING

#include "forward/forward_include.hlsli"
#include "include/lighting.hlsli"


float4 PS(PSPositionNormalTexture pin) : SV_Target {
	// The to_eye vector is used in lighting
	float3 to_eye = CameraPosition() - pin.position_world;

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

	float3 normal_vec = GetNormal(pin.position, pin.normal, pin.tex);


	//----------------------------------------------------------------------------------
	// Lighting
	//----------------------------------------------------------------------------------

	#ifdef ENABLE_LIGHTING
	float4 out_color = CalculateLighting(pin.position_world, normal_vec, to_eye, tex_color, mat);

	if (mat.reflection_enabled) {
		float3 incident = -to_eye;
		float3 reflection_vec = reflect(incident, pin.normal);
		float4 reflection_color = env_map.Sample(aniso_wrap, reflection_vec);

		out_color += mat.reflect * reflection_color;
	}
	#else
	float4 out_color = tex_color;
	#endif //ENABLE_LIGHTING


	//----------------------------------------------------------------------------------
	// Fogging
	//----------------------------------------------------------------------------------

	float fog_lerp = saturate((dist_to_eye - fog.start) / fog.range);

	// Blend the fog color and the lit color
	out_color = lerp(out_color, fog.color, fog_lerp);


	return saturate(out_color);
}