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

	// Calculate the position-to-eye vector
	float3 to_eye = CameraPosition() - pin.p_world;

	// Calculate distance to viewer
	float dist_to_eye = length(to_eye);

	// Normalize
	to_eye /= dist_to_eye;


	//----------------------------------------------------------------------------------
	// Create the material
	//----------------------------------------------------------------------------------
	float4 base_color = GetBaseColor(pin.uv);
	const float2 params = GetMaterialParams(pin.uv);

	Material mat;
	mat.base_color = base_color;
	mat.metalness = params.x;
	mat.roughness = params.y;
	mat.emissive = float3(0.0f, 0.0f, 0.0f);


	// Test alpha if transparency is enabled, or set it to 1 if not.
	#ifdef ENABLE_TRANSPARENCY
	clip(base_color.w - ALPHA_MIN);
	#else 
	base_color.w = 1.0f;
	#endif
	

	//----------------------------------------------------------------------------------
	// Lighting
	//----------------------------------------------------------------------------------

	#ifdef ENABLE_LIGHTING
	const float3 normal_vec = GetNormal(pin.p.xyz, pin.n, pin.uv);

	float3 out_color = CalculateLighting(pin.p_world, normal_vec, to_eye, mat);

	/*
	if (g_material.mirror_surface) {
		const float3 incident         = -to_eye;
		const float3 reflection_vec   = reflect(incident, pin.n);
		const float3 reflection_color = g_material.diffuse.xyz * env_map.Sample(g_aniso_wrap, reflection_vec).xyz;

		out_color.xyz = (g_material.reflectivity * reflection_color) + ((1.0f - g_material.reflectivity) * out_color.xyz);
	}
	*/
	#else  //ENABLE_LIGHTING
	float3 out_color = mat.base_color.xyz;
	#endif //ENABLE_LIGHTING


	//----------------------------------------------------------------------------------
	// Fogging
	//----------------------------------------------------------------------------------

	float fog_lerp = saturate((dist_to_eye - g_fog.start) / g_fog.range);

	// Blend the fog color and the lit color
	out_color = lerp(out_color, g_fog.color.xyz, fog_lerp);


	//----------------------------------------------------------------------------------
	// Final Value
	//----------------------------------------------------------------------------------
	
	// Apply the correct alpha and saturate
	return saturate(float4(out_color, base_color.w));
}