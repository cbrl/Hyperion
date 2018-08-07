#include "depth/depth_include.hlsli"
#include "include/transform.hlsli"


PSTexture VS(VSPositionNormalTexture vin) {

	PSTexture vout;
	vout.position = Transform(vin.position,
	                          model_to_world,
	                          world_to_camera_alt,
	                          camera_to_projection_alt);

	vout.tex = Transform(vin.tex, tex_transform);

	return vout;
}