#include "deferred/deferred_include.hlsli"
#include "include/lighting.hlsli"


float4 PS(float4 pin : SV_Position) : SV_Target {

	//----------------------------------------------------------------------------------
	// Create the material
	//----------------------------------------------------------------------------------
	const float3 p_world         = GetGBufferPosition(pin.xy);
	const float3 base_color      = GetGBufferBaseColor(pin.xy);
	const float2 material_params = GetGBufferMaterialParams(pin.xy);
	const float3 normal          = GetGBufferNormal(pin.xy);

	Material mat;
	mat.base_color = float4(base_color, 1.0f);
	mat.metalness  = material_params.x;
	mat.roughness  = material_params.y;
	mat.emissive   = 0.0f;
	

	//----------------------------------------------------------------------------------
	// Lighting
	//----------------------------------------------------------------------------------
	float3 out_color = 0.0f;

	#ifdef DISABLE_LIGHTING
	out_color = mat.base_color.xyz;
	#else  //DISABLE_LIGHTING
	out_color = Lighting::CalculateLighting(p_world, normal, mat);
	#endif //DISABLE_LIGHTING


	return saturate(float4(out_color, 1.0f));
}