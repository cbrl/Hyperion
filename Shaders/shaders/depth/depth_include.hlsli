#include "include/model.hlsli"


cbuffer AltCamera : REG_B(SLOT_CBUFFER_CAMERA_ALT) {

	matrix world_to_camera_alt;
	matrix camera_to_projection_alt;
}