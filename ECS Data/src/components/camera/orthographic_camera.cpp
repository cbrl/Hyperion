#include "orthographic_camera.h"


OrthographicCamera::OrthographicCamera(ID3D11Device& device,
                                       u32 viewport_width,
                                       u32 viewport_height)
	: CameraBase(device)
	, ortho_size(1.0f, 1.0f) {

	viewport.setSize(viewport_width, viewport_height);
}


XMMATRIX XM_CALLCONV OrthographicCamera::getCameraToProjectionMatrix() const {
	// Recalculate the projection matrix
	return XMMatrixOrthographicLH(ortho_size.x, ortho_size.y, depth.x, depth.y);
}
