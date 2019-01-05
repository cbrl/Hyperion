#include "orthographic_camera.h"


OrthographicCamera::OrthographicCamera(ID3D11Device& device, vec2_u32 viewport_size)
	: CameraBase(device)
	, ortho_size(1.0f, 1.0f) {

	viewport.setSize(viewport_size);
}

vec2_f32 OrthographicCamera::getSize() const noexcept {
	return ortho_size;
}

void OrthographicCamera::setSize(vec2_f32 size) noexcept {
	ortho_size = size;
}

XMMATRIX XM_CALLCONV OrthographicCamera::getCameraToProjectionMatrix() const {
	// Recalculate the projection matrix
	return XMMatrixOrthographicLH(ortho_size[0], ortho_size[1], depth[0], depth[1]);
}
