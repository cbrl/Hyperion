#include "include/global.hlsli"
#include "shapes/geosphere.hlsli"


PSWorldPosition VS(uint vertex : SV_VertexID) {
	PSWorldPosition vout;

	vout.position_world = geosphere[vertex];
	const float3 p      = mul(vout.position_world, (float3x3)g_world_to_camera);
	vout.position       = mul(float4(p, 1.0f), g_camera_to_projection).xyww;

	return vout;
}