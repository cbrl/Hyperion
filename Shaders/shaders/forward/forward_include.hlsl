#include "include/global.hlsl"
#include "include/input_structs.hlsl"
#include "include/normal.hlsl"
#include "include/light.hlsl"


cbuffer Model : REG_B(SLOT_CBUFFER_MODEL) {

	matrix   model_to_world;
	matrix   world_inv_transpose;
	matrix   tex_transform;
	Material mat;
};