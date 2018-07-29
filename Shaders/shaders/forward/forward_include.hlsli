#include "include/global.hlsl"
#include "include/model.hlsl"
#include "include/normal.hlsl"


//----------------------------------------------------------------------------------
//  SRVs
//----------------------------------------------------------------------------------

// Environment map texture
TextureCube env_map : REG_T(SLOT_SRV_ENV_MAP);

// Model textures
Texture2D diffuse_map : REG_T(SLOT_SRV_DIFFUSE);
Texture2D normal_map  : REG_T(SLOT_SRV_NORMAL);
// ambient, specular, etc...



float3 GetNormal(float3 position, float3 normal, float2 tex) {

	float3 sam = normal_map.Sample(linear_wrap, tex).xyz;
	
	return sam.z ? TransformNormal(position, normal, tex, sam) : normalize(normal);
}