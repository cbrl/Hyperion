#include "stdafx.h"
#include "camera_transform.h"


CameraTransform::CameraTransform()
	: parent(handle64::invalid_handle)

	, world(XMMatrixIdentity())
	, position(XMVectorZero())

	, right({ 1.0f, 0.0f, 0.0f, 0.0f })
	, up({ 0.0f, 1.0f, 0.0f, 0.0f })
	, forward({ 0.0f, 0.0f, 1.0f, 0.0f })

	, pitch(0.0f)
	, max_pitch(XMConvertToRadians(89.0f))
	, yaw(0.0f)

	, needs_update(false)
	, updated(false) {
}


void CameraTransform::SetRotation(const float3& rotation) {
	XMMATRIX mat = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

    right     = mat.r[0];
	up        = mat.r[1];
	forward   = mat.r[2];

	needs_update = true;
}


void XM_CALLCONV CameraTransform::SetRotation(FXMVECTOR rotation) {
	XMMATRIX mat = XMMatrixRotationRollPitchYawFromVector(rotation);

    right     = mat.r[0];
	up        = mat.r[1];
	forward   = mat.r[2];

	needs_update = true;
}


void CameraTransform::Rotate(const float3& units) {

	if (units.x) RotateX(units.x);
	if (units.y) RotateY(units.y);
	if (units.z) RotateZ(units.z);

	// Calculate the new pitch, yaw, and roll values.
	// lookLengthXZ is used in place of camera_forward.z for the pitch. This causes
	// the pitch to go 0->90->0 instead of 0->180, which makes it easy to clamp
	float3 forward_f32;
	XMStoreFloat3(&forward_f32, forward);

	float lookLengthXZ = sqrtf(powf(forward_f32.x, 2) + powf(forward_f32.z, 2));
	pitch = atan2f(forward_f32.y, lookLengthXZ);
	yaw   = atan2f(forward_f32.x, forward_f32.z);

	needs_update = true;
}


void CameraTransform::RotateX(const float units) {
	XMMATRIX x_rotation = XMMatrixRotationAxis(right, units);

	up = XMVector3TransformNormal(up, x_rotation);
	forward = XMVector3TransformNormal(forward, x_rotation);
}


void CameraTransform::RotateY(const float units) {
	XMMATRIX y_rotation = XMMatrixRotationAxis(up, units);

	right     = XMVector3TransformNormal(right, y_rotation);
	forward   = XMVector3TransformNormal(forward, y_rotation);
}


void CameraTransform::RotateZ(const float units) {
	XMMATRIX z_rotation = XMMatrixRotationAxis(forward, units);

	right = XMVector3TransformNormal(right, z_rotation);
	up    = XMVector3TransformNormal(up, z_rotation);
}


void CameraTransform::SetPosition(const float3& position) {
	this->position = XMLoadFloat3(&position);
	needs_update = true;
}


void XM_CALLCONV CameraTransform::SetPosition(FXMVECTOR position) {
	this->position = position;
	needs_update = true;
}


void CameraTransform::Move(const float3 & units) {
	position += XMLoadFloat3(&units);
	needs_update = true;
}


void XM_CALLCONV CameraTransform::Move(FXMVECTOR units) {
	position += units;
	needs_update = true;
}
