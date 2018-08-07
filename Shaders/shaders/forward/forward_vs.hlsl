#include "forward/forward_include.hlsli"
#include "include/transform.hlsli"


PSPositionNormalTexture VS(VSPositionNormalTexture vin) {

	return Transform(vin,
					 model_to_world,
					 world_to_camera,
					 camera_to_projection,
					 world_inv_transpose,
					 tex_transform);
}