#include "stdafx.h"
#include "Camera.h"


Camera::Camera() :
	enableFreeLook(true),

	lookAt(XMVectorZero()),
	position(XMVectorZero()),
	velocity(0.0f, 0.0f, 0.0f),
	moveAccel(0.0001f),
	moveDecel(0.0001f),
	maxVelocity(0.02f),
	isMoving(false),

	turnFactor(0.002f),
	pitch(0.0f),
	yaw(0.0f),
	roll(0.0f),
	maxPitch(XMConvertToRadians(89.0f)),

	cameraForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	cameraRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	cameraUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
	defaultForward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
	defaultRight(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
	defaultUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),

	viewMatrix(XMVectorZero(), XMVectorZero(), XMVectorZero(), XMVectorZero())
{
}


Camera::~Camera() {
}


void Camera::Move(XMFLOAT3 units) {
	// IsMoving determines if the camera will decelerate when Update is called
	isMoving = true;

	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (copysign(1.0f, units.x) == copysign(1.0f, velocity.x)) {
		velocity.x += units.x * moveAccel;
	}
	else {
		velocity.x = units.x * moveAccel;
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (copysign(1.0f, units.y) == copysign(1.0f, velocity.y)) {
		velocity.y += units.y * moveAccel;
	}
	else {
		velocity.y = units.y * moveAccel;
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (copysign(1.0f, units.z) == copysign(1.0f, velocity.z)) {
		velocity.z += units.z * moveAccel;
	}
	else {
		velocity.z = units.z * moveAccel;
	}
}


void Camera::Rotate(XMFLOAT3 units) {
	//----------------------------------------------------------------------------------
	// X rotation (Pitch)
	//----------------------------------------------------------------------------------
	if (units.x) {
		float xUnits = units.x * turnFactor;

		if (!enableFreeLook) {
			pitch -= xUnits;
			if (pitch > maxPitch) {
				xUnits += pitch - maxPitch;
			}
			else if (pitch < -maxPitch) {
				xUnits += pitch + maxPitch;
			}
		}

		XMMATRIX xRotation = XMMatrixRotationAxis(cameraRight, (xUnits));

		if (enableFreeLook) {
			cameraUp = XMVector3TransformNormal(cameraUp, xRotation);
		}
		cameraForward = XMVector3TransformNormal(cameraForward, xRotation);
	}


	//----------------------------------------------------------------------------------
	// Y rotation (Yaw)
	//----------------------------------------------------------------------------------
	if (units.y) {
		XMMATRIX yRotation = XMMatrixRotationAxis(cameraUp, (units.y * turnFactor));
		cameraRight = XMVector3TransformNormal(cameraRight, yRotation);
		cameraForward = XMVector3TransformNormal(cameraForward, yRotation);
	}


	//----------------------------------------------------------------------------------
	// Z rotation (Roll)
	//----------------------------------------------------------------------------------
	if (units.z && enableFreeLook) {
		XMMATRIX zRotation = XMMatrixRotationAxis(cameraForward, (units.z * turnFactor));
		cameraRight = XMVector3TransformNormal(cameraRight, zRotation);
		cameraUp = XMVector3TransformNormal(cameraUp, zRotation);
	}
}


void Camera::Update(float deltaTime) {
	XMVECTOR velocityVec = XMLoadFloat3(&velocity);

	// Limit veloctiy to maximum
	if (XMVectorGetX(XMVector3Length(velocityVec)) > maxVelocity) {

		velocityVec = XMVector3Normalize(velocityVec) * maxVelocity;
		XMStoreFloat3(&velocity, velocityVec);
	}


	// Move camera
	position += cameraRight   * velocity.x * deltaTime;
	position += cameraUp      * velocity.y * deltaTime;
	position += cameraForward * velocity.z * deltaTime;


	// Update position buffer
	buffer.position.x = XMVectorGetX(position);
	buffer.position.y = XMVectorGetY(position);
	buffer.position.z = XMVectorGetZ(position);


	// Add the position and forward vectors of the camera to the target vector
	lookAt = position + cameraForward;


	// Create the new view matrix
	viewMatrix = XMMatrixLookAtLH(position, lookAt, cameraUp);


	// Calculate the new pitch, yaw, and roll values.
	// lookLengthXZ is used in place of cameraForward.z for the pitch. This ensures the
	// pitch does not exceed +-90 degrees, which is needed for limiting the max pitch.
	float lookLengthXZ = sqrtf(powf(XMVectorGetX(cameraForward), 2) + powf(XMVectorGetZ(cameraForward), 2));
	pitch = atan2f(XMVectorGetY(cameraForward), lookLengthXZ);
	yaw   = atan2f(XMVectorGetX(cameraForward), XMVectorGetZ(cameraForward));
	roll  = atan2f(XMVectorGetX(cameraUp), XMVectorGetY(cameraUp));


	// Decelerate if not moving
	if (!isMoving) {
		float deceleration;

		if (velocity.x != 0.0f) {
			deceleration = copysign(1.0f, velocity.x) * moveDecel * deltaTime;

			if (abs(deceleration) > abs(velocity.x)) {
				velocity.x = 0.0f;
			}
			else {
				velocity.x -= deceleration;
			}
		}

		if (velocity.y != 0.0f) {
			deceleration = copysign(1.0f, velocity.y) * moveDecel * deltaTime;

			if (abs(deceleration) > abs(velocity.y)) {
				velocity.y = 0.0f;
			}
			else {
				velocity.y -= deceleration;
			}
		}

		if (velocity.z != 0.0f) {
			deceleration = copysign(1.0f, velocity.z) * moveDecel * deltaTime;

			if (abs(deceleration) > abs(velocity.z)) {
				velocity.z = 0.0f;
			}
			else {
				velocity.z -= deceleration;
			}
		}
	}

	// Set IsMoving to false. Will be set to true if camera moves again before update.
	isMoving = false;
}
