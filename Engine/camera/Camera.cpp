#include "stdafx.h"
#include "Camera.h"


Camera::Camera() :
	m_EnableFreeLook(true),

	m_LookAt(XMVectorZero()),
	m_Position(XMVectorZero()),
	m_MoveSpeed(0.0f, 0.0f, 0.0f),
	m_MoveAccel(0.0003f),
	m_MoveDecel(0.003f),
	m_MaxMoveSpeed(0.015f),

	m_Rotation(0.0f, 0.0f, 0.0f),
	m_TurnFactor(0.002f),
	m_Pitch(0.0f),
	m_Yaw(0.0f),
	m_Roll(0.0f),
	m_MaxPitch(89.0f),

	m_CameraForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_CameraRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	m_CameraUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
	m_DefaultForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_DefaultRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	m_DefaultUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
{
}


Camera::~Camera() {
}


void Camera::Move(XMINT3 directions, float deltaTime) {
	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	m_MoveSpeed.x += directions.x * m_MoveAccel * deltaTime;
	if (directions.x == 0) {
		if (m_MoveSpeed.x != 0.0f) {
			m_MoveSpeed.x -= copysign(1.0f, m_MoveSpeed.x) * m_MoveDecel * deltaTime;
			if (abs(m_MoveSpeed.x) <= 0.001f) m_MoveSpeed.x = 0.0f;
		}
	}

	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	m_MoveSpeed.y += directions.y * m_MoveAccel * deltaTime;
	if (directions.y == 0) {
		if (m_MoveSpeed.y != 0.0f) {
			m_MoveSpeed.y -= copysign(1.0f, m_MoveSpeed.y) * m_MoveDecel * deltaTime;
			if (abs(m_MoveSpeed.y) <= 0.001f) m_MoveSpeed.y = 0.0f;
		}
	}

	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	m_MoveSpeed.z += directions.z * m_MoveAccel * deltaTime;
	if (directions.z == 0) {
		if (m_MoveSpeed.z != 0.0f) {
			m_MoveSpeed.z -= copysign(1.0f, m_MoveSpeed.z) * m_MoveDecel * deltaTime;
			if (abs(m_MoveSpeed.z) <= 0.001f) m_MoveSpeed.z = 0.0f;
		}
	}
}


void Camera::Rotate(XMINT3 directions, float deltaTime) {
	// Pitch
	m_Pitch += directions.x * m_TurnFactor;

	if (m_Pitch > XM_PI) {
		m_Pitch = -XM_PI;
	}
	else if (m_Pitch < -XM_PI) {
		m_Pitch = XM_PI;
	}


	// Yaw
	m_Yaw += directions.y * m_TurnFactor;

	if (m_Yaw > XM_PI) {
		m_Yaw = -XM_PI;
	}
	else if (m_Yaw < -XM_PI) {
		m_Yaw = XM_PI;
	}


	// Roll
	if (m_EnableFreeLook) {
		m_Roll += directions.z * m_TurnFactor;

		if (m_Roll > XM_PI) {
			m_Roll = -XM_PI;
		}
		else if (m_Roll < -XM_PI) {
			m_Roll = XM_PI;
		}
	}
}


void Camera::Update() {
	XMVECTOR velocity = XMLoadFloat3(&m_MoveSpeed);

	// Limit veloctiy to maximum
	if (XMVectorGetX(XMVector3Length(velocity)) > m_MaxMoveSpeed) {
		velocity = XMVector3Normalize(velocity) * m_MaxMoveSpeed;
	}

	// Create rotation matrix and use it transfrom lookAt vector
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll);
	m_LookAt = XMVector3TransformCoord(m_DefaultForward, rotationMatrix);
	m_LookAt = XMVector3Normalize(m_LookAt);

	// Update camera vectors using rotation matrix
	if (m_EnableFreeLook) {
		m_CameraRight   = XMVector3TransformCoord(m_DefaultRight, rotationMatrix);
		m_CameraForward = XMVector3TransformCoord(m_DefaultForward, rotationMatrix);
		m_CameraUp      = XMVector3TransformCoord(m_DefaultUp, rotationMatrix);
	}
	else {
		XMMATRIX yRotationMatrix = XMMatrixRotationY(m_Yaw);
		m_CameraRight   = XMVector3TransformCoord(m_DefaultRight, yRotationMatrix);
		m_CameraForward = XMVector3TransformCoord(m_DefaultForward, yRotationMatrix);
		m_CameraUp      = XMVector3TransformCoord(m_DefaultUp, yRotationMatrix);
	}

	// Move camera
	m_Position += m_CameraRight * XMVectorGetX(velocity);
	m_Position += m_CameraUp * XMVectorGetY(velocity);
	m_Position += m_CameraForward * XMVectorGetZ(velocity);

	// Update position buffer
	m_Buffer.position.x = XMVectorGetX(m_Position);
	m_Buffer.position.y = XMVectorGetY(m_Position);
	m_Buffer.position.z = XMVectorGetZ(m_Position);

	// Add the position of the camera to the target vector
	m_LookAt += m_Position;

	// Create the new view matrix
	m_ViewMatrix = XMMatrixLookAtLH(m_Position, m_LookAt, m_CameraUp);
}
