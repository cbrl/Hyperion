#include "stdafx.h"
#include "orthographic_camera.h"


OrthographicCamera::OrthographicCamera(ID3D11Device* device,
									   ID3D11DeviceContext* device_context,
									   u32 viewport_width,
									   u32 viewport_height)
	: CameraBase(device,
				 device_context,
				 viewport_width,
				 viewport_height) {

	ResizeViewport(device_context, viewport_width, viewport_height);
}


void OrthographicCamera::UpdateViewMatrix() {
	// Create a target vector with the sum of the position and forward vectors
	XMVECTOR look_at = position + camera_forward;

	// Create the new view matrix
	view_matrix = XMMatrixLookAtLH(position, look_at, camera_up);
}


void OrthographicCamera::UpdateProjectionMatrix() {
	// Recalculate the projection matrix
	projection_matrix = XMMatrixOrthographicLH(viewport.Width, viewport.Height, z_near, z_far);
}


void OrthographicCamera::SetRotation(const float3& rotation) {
	pitch = rotation.x;
	yaw   = rotation.y;
	roll  = rotation.z;

	// Transform the forward/right/up vectors before updating the view matrix
	XMMATRIX M = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	camera_forward = XMVector3TransformNormal(default_forward, M);
	camera_right   = XMVector3TransformNormal(default_right, M);
	camera_up      = XMVector3TransformNormal(default_up, M);

	UpdateViewMatrix();
}