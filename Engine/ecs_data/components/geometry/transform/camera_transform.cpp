#include "stdafx.h"
#include "camera_transform.h"


CameraTransform::CameraTransform()
	: parent(Handle64::invalid_handle)

	, world(XMMatrixIdentity())

	, position(XMVectorZero())

	, right({ 1.0f, 0.0f, 0.0f, 0.0f })
	, up({ 0.0f, 1.0f, 0.0f, 0.0f })
	, forward({ 0.0f, 0.0f, 1.0f, 0.0f })

	, pitch(0.0f)
	, max_pitch(XMConvertToRadians(89.0f))
	, yaw(0.0f)
	, roll(0.0f)

	, free_look(true)

	, needs_update(false)
	, updated(false) {
}


void CameraTransform::SetRotation(const float3& rotation) {
	XMMATRIX mat = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	right   = mat.r[0];
	up      = mat.r[1];
	forward = mat.r[2];

	needs_update = true;
}


void XM_CALLCONV CameraTransform::SetRotation(FXMVECTOR rotation) {
	XMMATRIX mat = XMMatrixRotationRollPitchYawFromVector(rotation);

	right   = mat.r[0];
	up      = mat.r[1];
	forward = mat.r[2];

	needs_update = true;
}


void CameraTransform::Rotate(const float3& units) {

	RotateX(units.x);

	RotateY(units.y);

	if (free_look) {
		RotateZ(units.z);
	}

	// Calculate the new pitch, yaw, and roll values.
	// lookLengthXZ is used in place of camera_forward.z for the pitch. This causes
	// the pitch to go 0->90->0 instead of 0->180, which makes it easy to clamp
	float3 forward_f32;
	XMStoreFloat3(&forward_f32, forward);

	float lookLengthXZ = sqrtf(powf(forward_f32.x, 2) + powf(forward_f32.z, 2));
	pitch = atan2f(forward_f32.y, lookLengthXZ);
	yaw   = atan2f(forward_f32.x, forward_f32.z);
	roll  = atan2f(forward_f32.x, XMVectorGetY(up));

	needs_update = true;
}


void CameraTransform::RotateX(const float units) {

	float x_units = units;

	// Limit max pitch if free look isn't enabled
	if (!free_look) {
		pitch -= units;


		if (pitch > max_pitch) {
			x_units += pitch - max_pitch;
		}
		else if (pitch < -max_pitch) {
			x_units += pitch + max_pitch;
		}
	}

	XMMATRIX xRotation = XMMatrixRotationAxis(right, x_units);

	// Transform Up vector only if free look is enabled
	if (free_look) {
		up = XMVector3TransformNormal(up, xRotation);
	}

	forward = XMVector3TransformNormal(forward, xRotation);
}


void CameraTransform::RotateY(const float units) {
	XMMATRIX y_rotation = XMMatrixRotationAxis(up, units);

	right   = XMVector3TransformNormal(right, y_rotation);
	forward = XMVector3TransformNormal(forward, y_rotation);
}


void CameraTransform::RotateZ(const float units) {
	XMMATRIX z_rotation = XMMatrixRotationAxis(forward, units);

	right = XMVector3TransformNormal(right, z_rotation);
	up    = XMVector3TransformNormal(up, z_rotation);
}


void CameraTransform::Move(const float3 & units) {
	position += XMLoadFloat3(&units);
	needs_update = true;
}


void XM_CALLCONV CameraTransform::Move(FXMVECTOR units) {
	position += units;
	needs_update = true;
}
