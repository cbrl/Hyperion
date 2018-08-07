#include "depth/depth_include.hlsli"
#include "include/transform.hlsli"


float4 VS(VSPositionNormalTexture vin) : SV_POSITION {

	return Transform(vin.position,
					 model_to_world,
					 world_to_camera_alt,
					 camera_to_projection_alt);
}