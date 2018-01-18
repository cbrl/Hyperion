#include "stdafx.h"
#include "Camera.h"

// https://www.braynzarsoft.net/viewtutorial/q16390-19-first-person-camera
// http://www.chadvernon.com/blog/resources/directx9/moving-around-a-3d-world/
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb281696(v=vs.85).aspx

Camera::Camera() :
	m_MoveSpeed(0.0f, 0.0f, 0.0f),
	m_MoveAccel(0.0001f),
	m_MoveDecel(0.0005f),
	m_MaxMoveSpeed(0.01f),

	m_Rotation(0.0f, 0.0f, 0.0f),
	m_TurnSpeed(0.0f, 0.0f, 0.0f),
	m_TurnAccel(0.001f),
	m_TurnDecel(0.005f),
	m_MaxTurnSpeed(0.15f),

	m_DefaultForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_DefaultRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	m_CameraForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_CameraRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	m_CameraUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
{
}


Camera::~Camera() {
}


void Camera::Move(XMINT3 directions, float deltaTime) {
	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	m_MoveSpeed.x += directions.x * deltaTime * m_MoveAccel;
	if (directions.x > 0) {
		if (m_MoveSpeed.x > (deltaTime * m_MaxMoveSpeed)) {
			m_MoveSpeed.x = deltaTime * m_MaxMoveSpeed;
		}
	}
	else if (directions.x < 0) {
		if (m_MoveSpeed.x < (deltaTime * m_MaxMoveSpeed)) {
			m_MoveSpeed.x = -(deltaTime * m_MaxMoveSpeed);
		}
	}
	else {
		//m_MoveSpeed.x = 0.0f;
		if (m_MoveSpeed.x != 0.0f) {
			m_MoveSpeed.x -= copysign(1.0f, m_MoveSpeed.x) * m_MoveDecel;
			if (abs(m_MoveSpeed.x) <= 0.001f) m_MoveSpeed.x = 0.0f;
		}
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	m_MoveSpeed.y += directions.y * deltaTime * m_MoveAccel;
	if (directions.y > 0) {
		if (m_MoveSpeed.y > (deltaTime * m_MaxMoveSpeed)) {
			m_MoveSpeed.y = deltaTime * m_MaxMoveSpeed;
		}
	}
	else if (directions.y < 0) {
		if (m_MoveSpeed.y < (deltaTime * m_MaxMoveSpeed)) {
			m_MoveSpeed.y = -(deltaTime * m_MaxMoveSpeed);
		}
	}
	else {
		//m_MoveSpeed.y = 0.0f;
		if (m_MoveSpeed.y != 0.0f) {
			m_MoveSpeed.y -= copysign(1.0f, m_MoveSpeed.y) * m_MoveDecel;
			if (abs(m_MoveSpeed.y) <= 0.001f) m_MoveSpeed.y = 0.0f;
		}
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	m_MoveSpeed.z += directions.z * deltaTime * m_MoveAccel;
	if (directions.z > 0) {
		if (m_MoveSpeed.z > (deltaTime * m_MaxMoveSpeed)) {
			m_MoveSpeed.z = deltaTime * m_MaxMoveSpeed;
		}
	}
	else if (directions.z < 0) {
		if (m_MoveSpeed.z < (deltaTime * m_MaxMoveSpeed)) {
			m_MoveSpeed.z = -(deltaTime * m_MaxMoveSpeed);
		}
	}
	else {
		//m_MoveSpeed.z = 0.0f;
		if (m_MoveSpeed.z != 0.0f) {
			m_MoveSpeed.z -= copysign(1.0f, m_MoveSpeed.z) * m_MoveDecel;
			if (abs(m_MoveSpeed.z) <= 0.001f) m_MoveSpeed.z = 0.0f;
		}
	}
}


void Camera::Rotate(XMINT3 directions, float deltaTime) {
	//----------------------------------------------------------------------------------
	// X rotation
	//----------------------------------------------------------------------------------
	m_TurnSpeed.x += directions.x * deltaTime * m_TurnAccel;
	if (directions.x > 0) {
		if (m_TurnSpeed.x > (directions.x * m_MaxTurnSpeed)) {
			m_TurnSpeed.x = directions.x * m_MaxTurnSpeed;
		}
	}
	else if (directions.x < 0) {
		if (m_TurnSpeed.x < (directions.x * m_MaxTurnSpeed)) {
			m_TurnSpeed.x = directions.x * m_MaxTurnSpeed;
		}
	}
	else {
		//m_TurnSpeed.x = 0.0f;
		if (m_TurnSpeed.x != 0.0f) {
			m_TurnSpeed.x -= copysign(1.0f, m_TurnSpeed.x) * deltaTime * m_TurnDecel;
			if (abs(m_TurnSpeed.x) <= 0.01f) m_TurnSpeed.x = 0.0f;
		}
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
	m_TurnSpeed.y += directions.y * deltaTime * m_TurnAccel;
	if (directions.y > 0) {
		if (m_TurnSpeed.y > (directions.y * m_MaxTurnSpeed)) {
			m_TurnSpeed.y = directions.y * m_MaxTurnSpeed;
		}
	}
	else if (directions.y < 0) {
		if (m_TurnSpeed.y < (directions.y * m_MaxTurnSpeed)) {
			m_TurnSpeed.y = directions.y * m_MaxTurnSpeed;
		}
	}
	else {
		//m_TurnSpeed.y = 0.0f;
		if (m_TurnSpeed.y != 0.0f) {
			m_TurnSpeed.y -= copysign(1.0f, m_TurnSpeed.y) * deltaTime * m_TurnDecel;
			if (abs(m_TurnSpeed.y) <= 0.01f) m_TurnSpeed.y = 0.0f;
		}
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
	m_TurnSpeed.z += directions.x * deltaTime * m_TurnAccel;
	if (directions.z > 0) {
		if (m_TurnSpeed.z > (directions.z * m_MaxTurnSpeed)) {
			m_TurnSpeed.z = directions.z * m_MaxTurnSpeed;
		}
	}
	else if (directions.z < 0) {
		if (m_TurnSpeed.z < (directions.z * m_MaxTurnSpeed)) {
			m_TurnSpeed.z = directions.z * m_MaxTurnSpeed;
		}
	}
	else {
		//m_TurnSpeed.z = 0.0f;
		if (m_TurnSpeed.z != 0.0f) {
			m_TurnSpeed.z -= copysign(1.0f, m_TurnSpeed.z) * deltaTime * m_TurnDecel;
			if (abs(m_TurnSpeed.z) <= 0.01f) m_TurnSpeed.z = 0.0f;
		}
	}

	m_Rotation.z += m_TurnSpeed.z;

	if (m_Rotation.z > 360.0f) {
		m_Rotation.z = -360.0f;
	}
	else if (m_Rotation.z < -360.0f) {
		m_Rotation.z = 360.0f;
	}
}


void Camera::Update() {
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
	positionVector += m_CameraRight * m_MoveSpeed.x;
	positionVector += m_CameraForward * m_MoveSpeed.z;

	// Update camera buffer
	m_Buffer.position.x = XMVectorGetX(positionVector);
	m_Buffer.position.z = XMVectorGetZ(positionVector);

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
