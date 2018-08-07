#ifndef HLSL_MODEL
#define HLSL_MODEL

#include "hlsl.h"
#include "include/syntax.hlsli"
#include "include/material.hlsli"

cbuffer Model : REG_B(SLOT_CBUFFER_MODEL) {

	matrix   model_to_world;
	matrix   world_inv_transpose;
	matrix   tex_transform;
	Material mat;
};


#endif //HLSL_MODEL