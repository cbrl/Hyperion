//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// ENABLE_TRANSPARENCY
// ENABLE_LIGHTING


#include "forward/forward_include.hlsli"
#include "include/lighting.hlsli"


float4 PS(PSPositionNormalTexture pin) : SV_Target {

	// Calculate the position-to-eye vector
	float3 p_to_view = CameraPosition() - pin.p_world;

	// Calculate distance to viewer
	float dist_p_to_view = length(p_to_view);

	// Normalize
	p_to_view /= dist_p_to_view;


	//----------------------------------------------------------------------------------
	// Create the material
	//----------------------------------------------------------------------------------
	float4 base_color   = GetBaseColor(pin.uv);
	const float2 params = GetMaterialParams(pin.uv);

	Material mat;
	mat.base_color = base_color;
	mat.metalness  = params.x;
	mat.roughness  = params.y;
	mat.emissive   = g_material.emissive;


	// Test alpha if transparency is enabled, or set it to 1 if not.
	#ifdef ENABLE_TRANSPARENCY
	clip(base_color.w - ALPHA_MIN);
	#else
	base_color.w = 1.0f;
	#endif
	

	//----------------------------------------------------------------------------------
	// Lighting
	//----------------------------------------------------------------------------------
	float3 out_color = 0.0f;

	#ifdef DISABLE_LIGHTING
	out_color = mat.base_color.xyz;
	#else  //DISABLE_LIGHTING
	const float3 normal_vec = GetNormal(pin.p_world, pin.n, pin.uv);
	out_color = Lighting::CalculateLighting(pin.p_world, normal_vec, p_to_view, mat);

	/*
	if (g_material.mirror_surface) {
		const float3 incident         = -p_to_view;
		const float3 reflection_vec   = reflect(incident, pin.n);
		const float3 reflection_color = g_material.diffuse.xyz * env_map.Sample(g_aniso_wrap, reflection_vec).xyz;

		out_color.xyz = (g_material.reflectivity * reflection_color) + ((1.0f - g_material.reflectivity) * out_color.xyz);
	}
	*/
	#endif //DISABLE_LIGHTING


	//----------------------------------------------------------------------------------
	// Fogging
	//----------------------------------------------------------------------------------

	#ifndef DISABLE_FOG
	float fog_lerp = saturate((dist_p_to_view - g_fog.start) / g_fog.range);

	// Blend the fog color and the lit color
	out_color = lerp(out_color, g_fog.color.xyz, fog_lerp);
	#endif


	//----------------------------------------------------------------------------------
	// Final Value
	//----------------------------------------------------------------------------------
	
	// Apply the correct alpha and saturate
	return saturate(float4(out_color, base_color.w));
}