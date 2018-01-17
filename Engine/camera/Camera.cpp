#include "stdafx.h"
#include "Camera.h"

// http://www.chadvernon.com/blog/resources/directx9/moving-around-a-3d-world/
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb281696(v=vs.85).aspx

Camera::Camera() :
	m_Rotation(0.0f, 0.0f, 0.0f),
	m_MoveSpeed(0.0f, 0.0f, 0.0f),
	m_TurnSpeed(0.0f, 0.0f, 0.0f),
	m_DefaultForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_DefaultRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	m_CameraForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_CameraRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	m_CameraUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
{
}


Camera::~Camera() {
}


void Camera::SetPosition(XMFLOAT3 position) {
	m_Buffer.position = position;
}


void Camera::SetRotation(XMFLOAT3 rotation) {
	m_Rotation = rotation;
}


void Camera::Move(XMFLOAT3 speeds) {
	m_MoveSpeed.x += speeds.x;
	if (speeds.x > 0.0f) {
		if (m_MoveSpeed.x > (speeds.x * 10)) {
			m_MoveSpeed.x = speeds.x * 10;
		}
	}
	else if (speeds.x < 0.0f) {
		if (m_MoveSpeed.x < (speeds.x * 10)) {
			m_MoveSpeed.x = speeds.x * 10;
		}
	}
	else {
		m_MoveSpeed.x = 0.0f;
	}

	m_Buffer.position.x += m_MoveSpeed.x;
}


void Camera::Rotate(XMFLOAT3 speeds) {
	//----------------------------------------------------------------------------------
	// X rotation
	//----------------------------------------------------------------------------------
	m_TurnSpeed.x += speeds.x;
	if (speeds.x > 0.0f) {
		if (m_TurnSpeed.x > (speeds.x * 10)) {
			m_TurnSpeed.x = speeds.x * 10;
		}
	}
	else if (speeds.x < 0.0f) {
		if (m_TurnSpeed.x < (speeds.x * 10)) {
			m_TurnSpeed.x = speeds.x * 10;
		}
	}
	else {
		m_TurnSpeed.x = 0.0f;
	}

	m_Rotation.x += m_TurnSpeed.x;

	if (m_Rotation.x > 360.0f) {
		m_Rotation.x = -360.0f;
	}
	else if (m_Rotation.x < -360.0f) {
		m_Rotation.x = 360.0f;
	}


	//----------------------------------------------------------------------------------
	// Y rotation
	//----------------------------------------------------------------------------------
	m_TurnSpeed.y += speeds.y;
	if (speeds.y > 0.0f) {
		if (m_TurnSpeed.y > (speeds.y * 10)) {
			m_TurnSpeed.y = speeds.y * 10;
		}
	}
	else if (speeds.y < 0.0f) {
		if (m_TurnSpeed.y < (speeds.y * 10)) {
			m_TurnSpeed.y = speeds.y * 10;
		}
	}
	else {
		m_TurnSpeed.y = 0.0f;
	}

	m_Rotation.y += m_TurnSpeed.y;

	if (m_Rotation.y > 360.0f) {
		m_Rotation.y = -360.0f;
	}
	else if (m_Rotation.y < -360.0f) {
		m_Rotation.y = 360.0f;
	}


	//----------------------------------------------------------------------------------
	// Z rotation
	//----------------------------------------------------------------------------------
	m_TurnSpeed.z += speeds.z;
	if (speeds.z > 0.0f) {
		if (m_TurnSpeed.z > (speeds.z * 10)) {
			m_TurnSpeed.z = speeds.z * 10;
		}
	}
	else if (speeds.z < 0.0f) {
		if (m_TurnSpeed.z < (speeds.z * 10)) {
			m_TurnSpeed.z = speeds.z * 10;
		}
	}
	else {
		m_TurnSpeed.z = 0.0f;
	}

	m_Rotation.z += m_TurnSpeed.z;

	if (m_Rotation.z > 360.0f) {
		m_Rotation.z = -360.0f;
	}
	else if (m_Rotation.z < -360.0f) {
		m_Rotation.z = 360.0f;
	}
}


void Camera::Render() {
	float pitch, yaw, roll;
	
	// Create rotation matrix using pitch, yaw, and roll in radians
	pitch = m_Rotation.x * 0.0174532925f;
	yaw   = m_Rotation.y * 0.0174532925f;
	roll  = m_Rotation.z * 0.0174532925f;
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform lookAt and up vectors so the view is correctly rotated at the origin
	XMVECTOR forwardVector = XMVector3TransformCoord(m_DefaultForward, rotationMatrix);
	forwardVector          = XMVector3Normalize(forwardVector);

	XMMATRIX yRotationMatrix = XMMatrixRotationY(yaw);
	m_CameraForward = XMVector3TransformCoord(m_DefaultForward, yRotationMatrix);
	m_CameraRight   = XMVector3TransformCoord(m_DefaultRight, yRotationMatrix);
	m_CameraUp      = XMVector3TransformCoord(m_CameraUp, yRotationMatrix);

	// Create position vector
	XMVECTOR positionVector = XMLoadFloat3(&m_Buffer.position);

	// Translate rotated camera position to the viewer position
	forwardVector = XMVectorAdd(positionVector, forwardVector);

	// Create the view matrix using the new vectors
	m_ViewMatrix = XMMatrixLookAtLH(positionVector, forwardVector, m_CameraUp);
}


CameraBuffer Camera::GetBuffer() {
	return m_Buffer;
}


XMFLOAT3 Camera::GetPosition() {
	return m_Buffer.position;
}


XMFLOAT3 Camera::GetRotation() {
	return m_Rotation;
}


XMMATRIX Camera::GetViewMatrix() {
	return m_ViewMatrix;
}
