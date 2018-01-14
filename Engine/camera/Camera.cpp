#include "stdafx.h"
#include "Camera.h"


Camera::Camera() :
	m_Rotation(0.0f, 0.0f, 0.0f),
	m_MoveSpeed(0.0f, 0.0f, 0.0f),
	m_TurnSpeed(0.0f, 0.0f, 0.0f)
{
}


Camera::~Camera() {
}


void Camera::SetPosition(XMFLOAT3 position) {
	m_Buffer.cameraPosition = position;
}


void Camera::SetRotation(XMFLOAT3 rotation) {
	m_Rotation = rotation;
}


void Camera::Move(XMFLOAT3 rate) {

}


void Camera::Rotate(XMFLOAT3 rates) {
	//----------------------------------------------------------------------------------
	// X rotation
	//----------------------------------------------------------------------------------
	m_TurnSpeed.x += rates.x;
	if (rates.x > 0.0f) {
		if (m_TurnSpeed.x > (rates.x * 10)) {
			m_TurnSpeed.x = rates.x * 10;
		}
	}
	else if (rates.x < 0.0f) {
		if (m_TurnSpeed.x < (rates.x * 10)) {
			m_TurnSpeed.x = rates.x * 10;
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
	m_TurnSpeed.y += rates.y;
	if (rates.y > 0.0f) {
		if (m_TurnSpeed.y >(rates.y * 10)) {
			m_TurnSpeed.y = rates.y * 10;
		}
	}
	else if (rates.y < 0.0f) {
		if (m_TurnSpeed.y < (rates.y * 10)) {
			m_TurnSpeed.y = rates.y * 10;
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
	m_TurnSpeed.z += rates.z;
	if (rates.z > 0.0f) {
		if (m_TurnSpeed.z >(rates.z * 10)) {
			m_TurnSpeed.z = rates.z * 10;
		}
	}
	else if (rates.z < 0.0f) {
		if (m_TurnSpeed.z < (rates.z * 10)) {
			m_TurnSpeed.z = rates.z * 10;
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
