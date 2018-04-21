#include "stdafx.h"
#include "camera_base.h"


CameraBase::CameraBase(ID3D11Device* device,
					   ID3D11DeviceContext* device_context,
					   u32 viewport_width,
					   u32 viewport_height)
	: buffer(device)
	, viewport({ 0.0f, 0.0f, 0.0f, 0.f, 0.0f, 1.0f })
	, position(XMVectorZero())
	, pitch(0.0f)
	, yaw(0.0f)
	, roll(0.0f)

	, camera_forward(default_forward)
	, camera_right(default_right)
	, camera_up(default_up)

	, view_matrix(XMMatrixIdentity())
	, projection_matrix(XMMatrixIdentity())

	, z_near(0.1f)
	, z_far(1000.0f) {
}


void CameraBase::SetRotation(const float3& rotation) {
	pitch = rotation.x;
	yaw = rotation.y;
	roll = rotation.z;

	// Transform the forward/right/up vectors before updating the view matrix
	XMMATRIX M = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	camera_forward = XMVector3TransformNormal(default_forward, M);
	camera_right = XMVector3TransformNormal(default_right, M);
	camera_up = XMVector3TransformNormal(default_up, M);

	UpdateViewMatrix();
}


void CameraBase::UpdateViewMatrix() {
	// Create a target vector with the sum of the position and forward vectors
	XMVECTOR look_at = position + camera_forward;

	// Create the new view matrix
	view_matrix = XMMatrixLookAtLH(position, look_at, camera_up);

	// The frustum needs to be updated when the view matrix changes
	UpdateFrustum();
}