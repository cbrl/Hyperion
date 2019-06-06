//----------------------------------------------------------------------------------
// Settings
//----------------------------------------------------------------------------------
// ENABLE_TRANSPARENCY
// DISABLE_LIGHTING


#include "forward/forward_include.hlsli"
#include "include/lighting.hlsli"


float4 PS(PSPositionNormalTexture pin) : SV_Target {

	//----------------------------------------------------------------------------------
	// Create the material
	//----------------------------------------------------------------------------------
	float4 base_color   = GetBaseColor(pin.uv);
	const float2 params = GetMaterialParams(pin.uv);

	Material mat;
	mat.base_color = base_color;
	mat.roughness  = params.x;
	mat.metalness  = params.y;
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
	out_color = Lighting::CalculateLighting(pin.p_world, normal_vec, mat);
	#endif //DISABLE_LIGHTING


	//----------------------------------------------------------------------------------
	// Final Value
	//----------------------------------------------------------------------------------
	
	// Apply the correct alpha and saturate
	return saturate(float4(out_color, base_color.w));
}