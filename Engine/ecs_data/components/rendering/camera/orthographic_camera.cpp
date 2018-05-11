#include "stdafx.h"
#include "orthographic_camera.h"


OrthographicCamera::OrthographicCamera(ID3D11Device& device,
									   ID3D11DeviceContext& device_context,
									   u32 viewport_width,
									   u32 viewport_height)
	: CameraBase(device,
				 device_context,
				 viewport_width,
				 viewport_height) {

	ResizeViewport(device_context, viewport_width, viewport_height);
}


void OrthographicCamera::UpdateProjectionMatrix() {
	// Recalculate the projection matrix
	projection_matrix = XMMatrixOrthographicLH(viewport.Width, viewport.Height, z_near, z_far);

	// Update the frustum when the projection matrix changes
	UpdateFrustum();
}