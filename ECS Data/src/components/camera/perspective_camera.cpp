#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(ID3D11Device& device,
                                     u32 viewport_width,
                                     u32 viewport_height)
	: CameraBase(device)
	, fov(XM_PI / 4.0f) {

	resizeViewport(viewport_width, viewport_height);
}


void PerspectiveCamera::updateProjectionMatrix() {
	// Recalculate the aspect ratio
	const u32_2 size = viewport.getSize();
	const f32 aspect_ratio = static_cast<f32>(size.x) / static_cast<f32>(size.y);

	// Recalculate the projection matrix
	projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
}
