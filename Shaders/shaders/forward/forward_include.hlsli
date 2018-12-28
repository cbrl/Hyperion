#include "include/global.hlsli"
#include "include/model.hlsli"
#include "include/normal.hlsli"


//----------------------------------------------------------------------------------
//  SRVs
//----------------------------------------------------------------------------------

// Environment map texture
TextureCube env_map : REG_T(SLOT_SRV_ENV_MAP);

// Model textures
Texture2D base_color_map     : REG_T(SLOT_SRV_BASE_COLOR);
Texture2D normal_map         : REG_T(SLOT_SRV_NORMAL);
Texture2D material_param_map : REG_T(SLOT_SRV_MATERIAL_PARAMS);
Texture2D emissive_map       : REG_T(SLOT_SRV_EMISSIVE);
// ambient, specular, etc...



float3 GetNormal(float3 position, float3 normal, float2 tex) {
	const float3 sam = normal_map.Sample(g_linear_wrap, tex).xyz;
	
	// Simply normalize and return the input normal if there was no normal map
	return sam != float3(0.0f, 0.0f, 0.0f)
		? TransformNormal(position, normal, tex, sam)
		: normalize(normal);
}


float4 GetBaseColor(float2 uv) {
	const float4 sam = base_color_map.Sample(g_aniso_wrap, uv);
	return g_material.base_color * sam;
}


// returns [metalness, roughness]
float2 GetMaterialParams(float2 uv) {
	const float2 sam = material_param_map.Sample(g_linear_wrap, uv).xy;
	return float2(g_material.metalness, g_material.roughness) * sam;
}