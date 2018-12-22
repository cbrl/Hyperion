//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// ENABLE_TRANSPARENCY
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
	float4 base_color;
	float  alpha;
	{
		if (g_material.has_texture) {
			float4 tex_color = diffuse_map.Sample(g_aniso_wrap, pin.tex);

			base_color = tex_color * g_material.diffuse;
			alpha      = tex_color.w * g_material.opacity;
		}
		else {
			base_color = g_material.diffuse;
			alpha      = g_material.opacity;
		}

		// Test alpha if transparency is enabled, or set it to 1 if not.
		#ifdef ENABLE_TRANSPARENCY
		clip(alpha - ALPHA_MIN);
		#else 
		alpha = 1.0f;
		#endif
	}
	

	//----------------------------------------------------------------------------------
	// Lighting
	//----------------------------------------------------------------------------------

	#ifdef ENABLE_LIGHTING
	const float3 normal_vec = GetNormal(pin.position.xyz, pin.normal, pin.tex);

	float4 out_color = CalculateLighting(pin.position_world, normal_vec, to_eye, base_color, g_material);

	if (g_material.mirror_surface) {
		const float3 incident         = -to_eye;
		const float3 reflection_vec   = reflect(incident, pin.normal);
		const float3 reflection_color = g_material.diffuse.xyz * env_map.Sample(g_aniso_wrap, reflection_vec).xyz;

		out_color.xyz = (g_material.reflectivity * reflection_color) + ((1.0f - g_material.reflectivity) * out_color.xyz);
	}
	#else  //ENABLE_LIGHTING
	float4 out_color = base_color;
	#endif //ENABLE_LIGHTING


	//----------------------------------------------------------------------------------
	// Fogging
	//----------------------------------------------------------------------------------

	float fog_lerp = saturate((dist_to_eye - g_fog.start) / g_fog.range);

	// Blend the fog color and the lit color
	out_color = lerp(out_color, g_fog.color, fog_lerp);


	// Apply the correct alpha
	out_color.w = alpha;

	return saturate(out_color);
}