#include "orthographic_camera.h"


OrthographicCamera::OrthographicCamera(ID3D11Device& device, vec2_u32 viewport_size)
	: CameraBase(device)
	, ortho_size(1.0f, 1.0f) {

	viewport.setSize(viewport_size);
}


XMMATRIX XM_CALLCONV OrthographicCamera::getCameraToProjectionMatrix() const {
	// Recalculate the projection matrix
	return XMMatrixOrthographicLH(ortho_size.x, ortho_size.y, depth.x, depth.y);
}
