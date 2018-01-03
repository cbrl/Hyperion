#include "stdafx.h"
#include "Camera.h"


Camera::Camera() : m_Rotation(XMFLOAT3(0.0f, 0.0f, 0.0f)) {
}


Camera::~Camera() {
}


void Camera::SetPosition(XMFLOAT3 position) {
	m_Buffer.cameraPosition = position;
}


void Camera::SetRotation(XMFLOAT3 rotation) {
	m_Rotation = rotation;
}


void Camera::Render() {
	float pitch, yaw, roll;

	// Create vector pointing upwards
	XMFLOAT3 up(0.0f, 1.0f, 0.0f);
	XMVECTOR upVector = XMLoadFloat3(&up);

	// Set position where camera is looking by default
	XMFLOAT3 lookAt(0.0f, 0.0f, 1.0f);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	// Create position vector
	XMVECTOR positionVector = XMLoadFloat3(&m_Buffer.cameraPosition);
	
	// Create rotation matrix using pitch, yaw, and roll in radians
	pitch = m_Rotation.x * 0.0174532925f;
	yaw   = m_Rotation.y * 0.0174532925f;
	roll  = m_Rotation.z * 0.0174532925f;
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform lookAt and up vectors so the view is correctly rotated at the origin
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector     = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate rotated camera position to the viewer position
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Create the view matrix using the new vectors
	m_ViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}


CameraBuffer Camera::GetBuffer() {
	return m_Buffer;
}


XMFLOAT3 Camera::GetPosition() {
	return m_Buffer.cameraPosition;
}


XMFLOAT3 Camera::GetRotation() {
	return m_Rotation;
}


XMMATRIX Camera::GetViewMatrix() {
	return m_ViewMatrix;
}
