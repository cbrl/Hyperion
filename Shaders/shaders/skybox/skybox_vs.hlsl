#include "include/global.hlsl"
#include "include/input_structs.hlsl"
#include "shapes/geosphere.hlsli"


PSPosition VS(uint vertex : SV_VertexID) {
	PSPosition vout;

	vout.position_world = geosphere[vertex];
	const float3 p      = mul(float4(vout.position_world, 1.0f), (float3x3)world_to_camera);
	vout.position       = mul(float4(p, 1.0f), camera_to_projection).xyww;

	return vout;
}