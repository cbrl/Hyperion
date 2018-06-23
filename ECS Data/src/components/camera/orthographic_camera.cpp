#include "orthographic_camera.h"


OrthographicCamera::OrthographicCamera(ID3D11Device& device,
                                       ID3D11DeviceContext& device_context,
                                       u32 viewport_width,
                                       u32 viewport_height)
	: CameraBase(device)
	, ortho_size(1.0f, 1.0f) {

	resizeViewport(device_context, viewport_width, viewport_height);
}


void OrthographicCamera::updateProjectionMatrix() {
	// Recalculate the projection matrix
	projection_matrix = XMMatrixOrthographicLH(ortho_size.x, ortho_size.y, z_near, z_far);
}
