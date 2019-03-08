#ifndef HLSL_DEFERRED_INCLUDE
#define HLSL_DEFERRED_INCLUDE

//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------
#include "include/global.hlsli"
#include "include/normal.hlsli"


//----------------------------------------------------------------------------------
// SRVs
//----------------------------------------------------------------------------------
Texture2D        g_base_color_map     : REG_T(SLOT_SRV_BASE_COLOR);
Texture2D        g_material_param_map : REG_T(SLOT_SRV_MATERIAL_PARAMS);
Texture2D        g_normal_map         : REG_T(SLOT_SRV_NORMAL);
Texture2D<float> g_depth_map          : REG_T(SLOT_SRV_DEPTH);


//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

float3 GetGBufferBaseColor(uint2 coords) {
	return g_base_color_map[coords].xyz;
}

// returns [metalness, roughness]
float2 GetGBufferMaterialParams(uint2 coords) {
	return g_material_param_map[coords].xy;
}

float3 GetGBufferNormal(uint2 coords) {
	return g_normal_map[coords].xyz;
}

float3 GetGBufferPosition(uint2 coords) {
	const float  depth = g_depth_map[coords];
	const float3 p_cam = DisplayToCamera(coords, depth);
	return CameraToWorld(p_cam);
}


#endif //HLSL_DEFERRED_INCLUDE