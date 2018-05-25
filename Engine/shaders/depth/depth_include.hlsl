#include "shaders\forward\forward_include.hlsl"
#include "shaders\include\transform.hlsl"


CONSTANT_BUFFER(AltCamera, SLOT_CBUFFER_CAMERA_ALT) {

	matrix world_to_camera_alt;
	matrix camera_to_projection_alt;
}