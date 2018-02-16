#include "shaders\include\syntax.hlsl"
#include "shader\HLSL.h"

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

CONSTANT_BUFFER(matrix_buffer, SLOT_CBUFFER_MATRIX) {
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

CONSTANT_BUFFER(camera_buffer, SLOT_CBUFFER_CAMERA) {
	float3 camera_position;
	float  padding;
};