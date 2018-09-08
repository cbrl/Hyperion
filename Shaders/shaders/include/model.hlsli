#ifndef HLSL_MODEL
#define HLSL_MODEL

#include "hlsl.h"
#include "include/syntax.hlsli"
#include "include/material.hlsli"

cbuffer Model : REG_B(SLOT_CBUFFER_MODEL) {

	matrix   g_model_to_world;
	matrix   g_world_inv_transpose;
	matrix   g_tex_transform;
	Material g_material;
};


#endif //HLSL_MODEL