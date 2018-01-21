#include "stdafx.h"
#include "Camera.h"


Camera::Camera() :
	m_EnableFreeLook(true),

	m_LookAt(XMVectorZero()),
	m_Position(XMVectorZero()),
	m_Velocity(0.0f, 0.0f, 0.0f),
	m_MoveAccel(0.0001f),
	m_MoveDecel(0.0001f),
	m_MaxVelocity(0.02f),

	m_TurnFactor(0.002f),
	m_Pitch(0.0f),
	m_Yaw(0.0f),
	m_Roll(0.0f),
	m_MaxPitch(XMConvertToRadians(89.0f)),

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


void Camera::Move(XMFLOAT3 units) {
	// IsMoving determines if the camera will decelerate when Update is called
	m_IsMoving = true;

	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (copysign(1.0f, units.x) == copysign(1.0f, m_Velocity.x)) {
		m_Velocity.x += units.x * m_MoveAccel;
	}
	else {
		m_Velocity.x = units.x * m_MoveAccel;
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (copysign(1.0f, units.y) == copysign(1.0f, m_Velocity.y)) {
		m_Velocity.y += units.y * m_MoveAccel;
	}
	else {
		m_Velocity.y = units.y * m_MoveAccel;
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (copysign(1.0f, units.z) == copysign(1.0f, m_Velocity.z)) {
		m_Velocity.z += units.z * m_MoveAccel;
	}
	else {
		m_Velocity.z = units.z * m_MoveAccel;
	}
}


void Camera::Rotate(XMFLOAT3 units) {
	//----------------------------------------------------------------------------------
	// X rotation (Pitch)
	//----------------------------------------------------------------------------------
	if (units.x) {
		float xUnits = units.x * m_TurnFactor;

		if (!m_EnableFreeLook) {
			m_Pitch -= xUnits;
			if (m_Pitch > m_MaxPitch) {
				xUnits += m_Pitch - m_MaxPitch;
			}
			else if (m_Pitch < -m_MaxPitch) {
				xUnits += m_Pitch + m_MaxPitch;
			}
		}

		XMMATRIX xRotation = XMMatrixRotationAxis(m_CameraRight, (xUnits));

		if (m_EnableFreeLook) {
			m_CameraUp = XMVector3TransformNormal(m_CameraUp, xRotation);
		}
		m_CameraForward = XMVector3TransformNormal(m_CameraForward, xRotation);
	}


	//----------------------------------------------------------------------------------
	// Y rotation (Yaw)
	//----------------------------------------------------------------------------------
	if (units.y) {
		XMMATRIX yRotation = XMMatrixRotationAxis(m_CameraUp, (units.y * m_TurnFactor));
		m_CameraRight = XMVector3TransformNormal(m_CameraRight, yRotation);
		m_CameraForward = XMVector3TransformNormal(m_CameraForward, yRotation);
	}


	//----------------------------------------------------------------------------------
	// Z rotation (Roll)
	//----------------------------------------------------------------------------------
	if (units.z && m_EnableFreeLook) {
		XMMATRIX zRotation = XMMatrixRotationAxis(m_CameraForward, (units.z * m_TurnFactor));
		m_CameraRight = XMVector3TransformNormal(m_CameraRight, zRotation);
		m_CameraUp = XMVector3TransformNormal(m_CameraUp, zRotation);
	}
}


void Camera::Update(float deltaTime) {
	XMVECTOR velocityVec = XMLoadFloat3(&m_Velocity);

	// Limit veloctiy to maximum
	if (XMVectorGetX(XMVector3Length(velocityVec)) > m_MaxVelocity) {

		velocityVec = XMVector3Normalize(velocityVec) * m_MaxVelocity;
		XMStoreFloat3(&m_Velocity, velocityVec);
	}


	// Move camera
	m_Position += m_CameraRight   * m_Velocity.x * deltaTime;
	m_Position += m_CameraUp      * m_Velocity.y * deltaTime;
	m_Position += m_CameraForward * m_Velocity.z * deltaTime;


	// Update position buffer
	m_Buffer.position.x = XMVectorGetX(m_Position);
	m_Buffer.position.y = XMVectorGetY(m_Position);
	m_Buffer.position.z = XMVectorGetZ(m_Position);


	// Add the position and forward vectors of the camera to the target vector
	m_LookAt = m_Position + m_CameraForward;


	// Create the new view matrix
	m_ViewMatrix = XMMatrixLookAtLH(m_Position, m_LookAt, m_CameraUp);


	// Calculate the new pitch, yaw, and roll values
	float lookLengthXZ = sqrtf(powf(XMVectorGetX(m_CameraForward), 2) + powf(XMVectorGetZ(m_CameraForward), 2));
	m_Pitch = atan2f(XMVectorGetY(m_CameraForward), lookLengthXZ);
	m_Yaw   = atan2f(XMVectorGetX(m_CameraForward), XMVectorGetZ(m_CameraForward));
	m_Roll  = atan2f(XMVectorGetX(m_CameraUp), XMVectorGetY(m_CameraUp));


	// Decelerate if not moving
	if (!m_IsMoving) {
		float deceleration;

		if (m_Velocity.x != 0.0f) {
			deceleration = copysign(1.0f, m_Velocity.x) * m_MoveDecel * deltaTime;

			if (abs(deceleration) > abs(m_Velocity.x)) {
				m_Velocity.x = 0.0f;
			}
			else {
				m_Velocity.x -= deceleration;
			}
		}

		if (m_Velocity.y != 0.0f) {
			deceleration = copysign(1.0f, m_Velocity.y) * m_MoveDecel * deltaTime;

			if (abs(deceleration) > abs(m_Velocity.y)) {
				m_Velocity.y = 0.0f;
			}
			else {
				m_Velocity.y -= deceleration;
			}
		}

		if (m_Velocity.z != 0.0f) {
			deceleration = copysign(1.0f, m_Velocity.z) * m_MoveDecel * deltaTime;

			if (abs(deceleration) > abs(m_Velocity.z)) {
				m_Velocity.z = 0.0f;
			}
			else {
				m_Velocity.z -= deceleration;
			}
		}
	}

	// Set IsMoving to false. Will be set to true if camera moves again before update.
	m_IsMoving = false;
}
