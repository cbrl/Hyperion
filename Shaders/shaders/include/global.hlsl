#ifndef HLSL_GLOBAL
#define HLSL_GLOBAL


#include "include\syntax.hlsl"
#include "..\src\hlsl.h"


//----------------------------------------------------------------------------------
// Sampler States
//----------------------------------------------------------------------------------

SamplerState point_wrap   : REG_S(SLOT_SAMPLER_POINT_WRAP);
SamplerState point_clamp  : REG_S(SLOT_SAMPLER_POINT_CLAMP);
SamplerState linear_wrap  : REG_S(SLOT_SAMPLER_LINEAR_WRAP);
SamplerState linear_clamp : REG_S(SLOT_SAMPLER_LINEAR_CLAMP);
SamplerState aniso_wrap   : REG_S(SLOT_SAMPLER_ANISO_WRAP);
SamplerState aniso_clamp  : REG_S(SLOT_SAMPLER_ANISO_CLAMP);

SamplerComparisonState pcf_sampler : REG_S(SLOT_SAMPLER_PCF);



//----------------------------------------------------------------------------------
// Constant Buffers
//----------------------------------------------------------------------------------

//cbuffer EngineBuffer : REG_B(SLOT_CBUFFER_ENGINE) {
//}


// Camera fog
struct Fog {
	float4 color;
	float  start;
	float  range;
	float2 pad0;
};

cbuffer CameraBuffer : REG_B(SLOT_CBUFFER_CAMERA) {
	// The camera's position
	float3 camera_position;

	// Padding
	float cb_pad0;

	// The world-to-camera (world-to-view) matrix
	matrix world_to_camera;

	// The camera-to-projection (view-to-projection) matrix
	matrix camera_to_projection;

	// The fog the camera sees
	Fog fog;
};



#endif //HLSL_GLOBAL