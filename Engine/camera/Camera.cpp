#include "stdafx.h"
#include "Camera.h"


Camera::Camera() :
	m_Position(XMVectorZero()),
	m_MoveSpeed(XMVectorZero()),
	m_MoveAccel(0.01f),
	m_MoveDecel(0.05f),
	m_MaxMoveSpeed(0.015f),

	m_Rotation(0.0f, 0.0f, 0.0f),
	m_TurnAccel(0.001f),
	m_Pitch(0.0f),
	m_MaxPitch(89.0f),

	m_CameraForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	m_CameraRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	m_CameraUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
{
}


Camera::~Camera() {
}


void Camera::Move(XMFLOAT3 units) {

	//if (!(units.x | units.y | units.z)) {
	//	return;
	//}

	XMVECTOR vDirections = XMVectorSet(units.x, units.y, units.z, 0.0f);

	m_MoveSpeed += vDirections * m_MoveAccel;
}


void Camera::Rotate(XMFLOAT3 units) {
	//if (!(units.x | units.y | units.z)) {
	//	return;
	//}


	// Y rotation
	if (units.y) {
		XMMATRIX yRotation = XMMatrixRotationAxis(m_CameraUp, (units.y * m_TurnAccel));
		m_CameraRight = XMVector3TransformNormal(m_CameraRight, yRotation);
		m_CameraForward = XMVector3TransformNormal(m_CameraForward, yRotation);
	}


	// X rotation
	if (units.x) {
		float xUnits = units.x * m_TurnAccel;
		m_Pitch -= xUnits;
		if (m_Pitch > m_MaxPitch) {
			xUnits += m_Pitch - m_MaxPitch;
		}
		else if (m_Pitch < -m_MaxPitch) {
			xUnits += m_Pitch + m_MaxPitch;
		}
		XMMATRIX xRotation = XMMatrixRotationAxis(m_CameraRight, (xUnits));
		m_CameraUp = XMVector3TransformNormal(m_CameraUp, xRotation);
		m_CameraForward = XMVector3TransformNormal(m_CameraForward, xRotation);
	}


	// Z rotation
	if (units.z) {
		XMMATRIX zRotation = XMMatrixRotationAxis(m_CameraForward, (units.z * m_TurnAccel));
		m_CameraRight = XMVector3TransformNormal(m_CameraRight, zRotation);
		m_CameraUp = XMVector3TransformNormal(m_CameraUp, zRotation);
	}
}


void Camera::Update() {
	// Limit move speed to max velocity
	if (XMVectorGetX(XMVector3Length(m_MoveSpeed)) > m_MaxMoveSpeed) {
		m_MoveSpeed = XMVector3Normalize(m_MoveSpeed) * m_MaxMoveSpeed;
	}
	
	// Move camera
	m_Position = XMLoadFloat3(&m_Buffer.position);
	m_Position += m_CameraRight * XMVectorGetX(m_MoveSpeed);
	m_Position += m_CameraUp * XMVectorGetY(m_MoveSpeed);
	m_Position += m_CameraForward * XMVectorGetZ(m_MoveSpeed);

	// Update position buffer
	m_Buffer.position.x = XMVectorGetX(m_Position);
	m_Buffer.position.y = XMVectorGetY(m_Position);
	m_Buffer.position.z = XMVectorGetZ(m_Position);

	// Set the lookAt vector
	m_LookAt = m_Position + m_CameraForward;

	// Stop movement
	m_MoveSpeed = XMVectorZero();
	//m_MoveSpeed *= (m_MoveDecel * deltaTime);
	//if (XMVectorGetX(XMVector3Length(m_MoveSpeed)) < 0.01f) {
	//	m_MoveSpeed = XMVectorZero();
	//}

	// Calculate new view matrix
	m_ViewMatrix = XMMatrixLookAtLH(m_Position, m_LookAt, m_CameraUp);
	
	//m_CameraRight = XMVectorSet(m_ViewMatrix.r[1].m128_f32[1], m_ViewMatrix.r[2].m128_f32[1], m_ViewMatrix.r[3].m128_f32[1], 0.0f);
	//m_CameraUp = XMVectorSet(m_ViewMatrix.r[1].m128_f32[2], m_ViewMatrix.r[2].m128_f32[2], m_ViewMatrix.r[3].m128_f32[2], 0.0f);
	//m_CameraForward = XMVectorSet(m_ViewMatrix.r[1].m128_f32[3], m_ViewMatrix.r[2].m128_f32[3], m_ViewMatrix.r[3].m128_f32[3], 0.0f);

	float lookLengthXZ = sqrtf(powf(XMVectorGetX(m_CameraForward), 2) + powf(XMVectorGetZ(m_CameraForward), 2));
	m_Pitch = atan2f(XMVectorGetY(m_CameraForward), lookLengthXZ);
	m_Yaw = atan2f(XMVectorGetX(m_CameraForward), XMVectorGetZ(m_CameraForward));
}
