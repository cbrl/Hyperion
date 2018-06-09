#include "stdafx.h"
#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(ID3D11Device& device,
                                     ID3D11DeviceContext& device_context,
                                     u32 viewport_width,
                                     u32 viewport_height)
	: CameraBase(device)
	, fov(XM_PI / 4.0f) {

	ResizeViewport(device_context, viewport_width, viewport_height);
}


void PerspectiveCamera::UpdateProjectionMatrix() {
	// Recalculate the aspect ratio
	const float aspect_ratio = viewport.Width / viewport.Height;

	// Recalculate the projection matrix
	projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);

	// Update the frustum when the projection matrix changes
	UpdateFrustum();
}
