#include "include/global.hlsli"
#include "include/transform.hlsli"
#include "shapes/wireframe_cube.hlsli"


cbuffer Model : REG_B(SLOT_CBUFFER_MODEL) {
	matrix g_object_to_world;
}


float4 VS(uint vertex : SV_VertexID) : SV_Position {
	return Transform(wireframe_cube[vertex],
	                 g_object_to_world,
	                 g_world_to_camera,
	                 g_camera_to_projection);
}