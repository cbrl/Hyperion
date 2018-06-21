#include "include/global.hlsl"
#include "include/transform.hlsl"
#include "shapes/wireframe_cube.hlsl"


cbuffer Model : REG_B(SLOT_CBUFFER_MODEL) {
	matrix object_to_world;
}


float4 VS(uint vertex : SV_VertexID) : SV_Position {
	return Transform(wireframe_cube[vertex],
	                 object_to_world,
	                 world_to_camera,
	                 camera_to_projection);
}