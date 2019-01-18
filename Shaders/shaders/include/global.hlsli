#ifndef HLSL_GLOBAL
#define HLSL_GLOBAL

#include "hlsl.h"
#include "include/syntax.hlsli"
#include "include/input_structs.hlsli"


//----------------------------------------------------------------------------------
// Sampler States
//----------------------------------------------------------------------------------

SamplerState g_point_wrap   : REG_S(SLOT_SAMPLER_POINT_WRAP);
SamplerState g_point_clamp  : REG_S(SLOT_SAMPLER_POINT_CLAMP);
SamplerState g_linear_wrap  : REG_S(SLOT_SAMPLER_LINEAR_WRAP);
SamplerState g_linear_clamp : REG_S(SLOT_SAMPLER_LINEAR_CLAMP);
SamplerState g_aniso_wrap   : REG_S(SLOT_SAMPLER_ANISO_WRAP);
SamplerState g_aniso_clamp  : REG_S(SLOT_SAMPLER_ANISO_CLAMP);

SamplerComparisonState g_pcf_sampler : REG_S(SLOT_SAMPLER_PCF);



//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

//cbuffer EngineBuffer : REG_B(SLOT_CBUFFER_ENGINE) {
	//float g_dt;
//}


// Camera fog
struct Fog {
	float  density;
	float3 color;
};

cbuffer CameraBuffer : REG_B(SLOT_CBUFFER_CAMERA) {

	// The camera-to-world matrix
	matrix g_camera_to_world;

	// The world-to-camera (world-to-view) matrix
	matrix g_world_to_camera;

	// The camera-to-projection (view-to-projection) matrix
	matrix g_camera_to_projection;

	// The fog the camera sees
	Fog g_fog;
};


float3 CameraPosition() {
	return g_camera_to_world._41_42_43;
}


#endif //HLSL_GLOBAL