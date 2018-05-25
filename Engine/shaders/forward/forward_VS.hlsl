#include "shaders\forward\forward_include.hlsl"
#include "shaders\include\transform.hlsl"


PSPositionNormalTexture VS(VSPositionNormalTexture vin) {

	return Transform(vin,
					 object_to_world,
					 world_to_camera,
					 camera_to_projection,
					 world_inv_transpose,
					 tex_transform);
}