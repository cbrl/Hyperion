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


//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

//CONSTANT_BUFFER(EngineBuffer, SLOT_CBUFFER_MATRIX) {
//}

CONSTANT_BUFFER(CameraBuffer, SLOT_CBUFFER_CAMERA) {
	float3 camera_position;
	float  padding;
};



#endif //HLSL_GLOBAL