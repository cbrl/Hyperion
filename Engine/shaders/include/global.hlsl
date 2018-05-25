#ifndef HLSL_GLOBAL
#define HLSL_GLOBAL


#include "shaders\include\syntax.hlsl"
#include "shader\hlsl.h"


//----------------------------------------------------------------------------------
// Sampler States
//----------------------------------------------------------------------------------

SAMPLER_STATE(point_wrap,   SLOT_SAMPLER_POINT_WRAP);
SAMPLER_STATE(point_clamp,  SLOT_SAMPLER_POINT_CLAMP);
SAMPLER_STATE(linear_wrap,  SLOT_SAMPLER_LINEAR_WRAP);
SAMPLER_STATE(linear_clamp, SLOT_SAMPLER_LINEAR_CLAMP);
SAMPLER_STATE(aniso_wrap,   SLOT_SAMPLER_ANISO_WRAP);
SAMPLER_STATE(aniso_clamp,  SLOT_SAMPLER_ANISO_CLAMP);
SAMPLER_COMPARISON_STATE(pcf_sampler, SLOT_SAMPLER_PCF);



//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

//CONSTANT_BUFFER(EngineBuffer, SLOT_CBUFFER_ENGINE) {
//}


CONSTANT_BUFFER(CameraBuffer, SLOT_CBUFFER_CAMERA) {
	// The camera's position
	float3 camera_position;

	// Padding
	float padding;

	// The world-to-camera (world-to-view) matrix
	matrix world_to_camera;

	// The camera-to-projection (view-to-projection) matrix
	matrix camera_to_projection;
};



#endif //HLSL_GLOBAL