#include "depth/depth_include.hlsli"
#include "include/transform.hlsli"


float4 VS(VSPositionNormalTexture vin) : SV_POSITION {

	return Transform(vin.p,
					 g_model_to_world,
					 g_world_to_camera_alt,
					 g_camera_to_projection_alt);
}