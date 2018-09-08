#include "include/model.hlsli"


cbuffer AltCamera : REG_B(SLOT_CBUFFER_CAMERA_ALT) {

	matrix g_world_to_camera_alt;
	matrix g_camera_to_projection_alt;
}