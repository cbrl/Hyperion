#include "include/global.hlsli"
#include "shapes/geosphere.hlsli"


PSWorldPosition VS(uint vertex : SV_VertexID) {

	const float3 p_world = geosphere[vertex];
	const float3 p_view  = mul(p_world, (float3x3)g_world_to_camera);
	const float4 p_proj  = mul(float4(p_view, 1.0f), g_camera_to_projection).xyww;

	PSWorldPosition vout;
	vout.p_world = p_world;
	vout.p       = p_proj;

	return vout;
}