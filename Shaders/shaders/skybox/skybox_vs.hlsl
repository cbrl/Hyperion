#include "include/global.hlsli"
#include "shapes/geosphere.hlsli"


PSWorldPosition VS(uint vertex : SV_VertexID) {
	PSWorldPosition vout;

	vout.p_world = geosphere[vertex];
	const float3 p      = mul(vout.p_world, (float3x3)g_world_to_camera);
	vout.p       = mul(float4(p, 1.0f), g_camera_to_projection).xyww;

	return vout;
}