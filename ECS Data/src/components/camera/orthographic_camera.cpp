#include "orthographic_camera.h"


OrthographicCamera::OrthographicCamera(ID3D11Device& device,
                                       u32 viewport_width,
                                       u32 viewport_height)
	: CameraBase(device)
	, ortho_size(1.0f, 1.0f) {

	resizeViewport(viewport_width, viewport_height);
}


void OrthographicCamera::updateProjectionMatrix() {
	// Recalculate the projection matrix
	projection_matrix = XMMatrixOrthographicLH(ortho_size.x, ortho_size.y, z_near, z_far);
}
