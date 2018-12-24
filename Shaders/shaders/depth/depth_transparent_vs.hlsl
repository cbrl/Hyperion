#include "depth/depth_include.hlsli"
#include "include/transform.hlsli"


PSPositionTexture VS(VSPositionNormalTexture vin) {

	PSPositionTexture vout;

	vout.p = Transform(vin.p,
	                   g_model_to_world,
	                   g_world_to_camera_alt,
	                   g_camera_to_projection_alt);

	vout.uv = Transform(vin.uv, g_tex_transform);

	return vout;
}