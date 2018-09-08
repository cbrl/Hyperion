#include "forward/forward_include.hlsli"
#include "include/transform.hlsli"


PSPositionNormalTexture VS(VSPositionNormalTexture vin) {

	return Transform(vin,
					 g_model_to_world,
					 g_world_to_camera,
					 g_camera_to_projection,
					 g_world_inv_transpose,
					 g_tex_transform);
}