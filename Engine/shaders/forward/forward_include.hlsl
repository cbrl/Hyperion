#include "shaders\include\global.hlsl"
#include "shaders\include\input_structs.hlsl"
#include "shaders\include\normal.hlsl"
#include "shaders\include\light.hlsl"


CONSTANT_BUFFER(Model, SLOT_CBUFFER_MODEL) {
	matrix   object_to_world;
	matrix   world_inv_transpose;
	//matrix   world_view_proj;
	matrix   tex_transform;
	Material mat;
};