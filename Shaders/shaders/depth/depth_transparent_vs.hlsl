#include "depth/depth_include.hlsli"
#include "include/transform.hlsli"


PSTexture VS(VSPositionNormalTexture vin) {

	PSTexture vout;
	vout.position = Transform(vin.position,
	                          g_model_to_world,
	                          g_world_to_camera_alt,
	                          g_camera_to_projection_alt);

	vout.tex = Transform(vin.tex, g_tex_transform);

	return vout;
}