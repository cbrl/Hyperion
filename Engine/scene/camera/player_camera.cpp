#include "stdafx.h"
#include "player_camera.h"
#include "util\math\math.h"


PlayerCamera::PlayerCamera(ID3D11Device* device,
						   ID3D11DeviceContext* device_context,
						   u32 viewport_width,
						   u32 viewport_height)
	: PerspectiveCamera(device,
						device_context,
						viewport_width,
						viewport_height)

	, velocity(0.0f, 0.0f, 0.0f)
	, acceleration(0.00002f)
	, deceleration(0.00005f)
	, max_velocity(0.01f)
	, is_moving_x(false)
	, is_moving_y(false)
	, is_moving_z(false)

	, turn_factor(0.002f)
	, max_pitch(XMConvertToRadians(89.0f))
	, free_look(false)
{}


void PlayerCamera::Move(float3 units) {
	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (units.x) {

		// is_moving determines if the camera will decelerate when Update is called
		is_moving_x = true;

		if (copysign(1.0f, units.x) == copysign(1.0f, velocity.x)) {
			velocity.x += units.x * acceleration;
		}
		else {
			velocity.x = units.x * acceleration;
		}
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (units.y) {
		is_moving_y = true;

		if (copysign(1.0f, units.y) == copysign(1.0f, velocity.y)) {
			velocity.y += units.y * acceleration;
		}
		else {
			velocity.y = units.y * acceleration;
		}
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (units.z) {
		is_moving_z = true;

		if (copysign(1.0f, units.z) == copysign(1.0f, velocity.z)) {
			velocity.z += units.z * acceleration;
		}
		else {
			velocity.z = units.z * acceleration;
		}
	}
}


void PlayerCamera::Rotate(float3 units) {
	//----------------------------------------------------------------------------------
	// X rotation (Pitch)
	//----------------------------------------------------------------------------------
	if (units.x) {
		float xUnits = units.x * turn_factor;

		// Limit max pitch if free look isn't enabled
		if (!free_look) {
			pitch -= xUnits;
			if (pitch > max_pitch) {
				xUnits += pitch - max_pitch;
			}
			else if (pitch < -max_pitch) {
				xUnits += pitch + max_pitch;
			}
		}

		XMMATRIX xRotation = XMMatrixRotationAxis(camera_right, xUnits);

		// Transform Up vector only if free look is enabled
		if (free_look) {
			camera_up = XMVector3TransformNormal(camera_up, xRotation);
		}
		camera_forward = XMVector3TransformNormal(camera_forward, xRotation);
	}


	//----------------------------------------------------------------------------------
	// Y rotation (Yaw)
	//----------------------------------------------------------------------------------
	if (units.y) {
		XMMATRIX yRotation = XMMatrixRotationAxis(camera_up, (units.y * turn_factor));
		camera_right = XMVector3TransformNormal(camera_right, yRotation);
		camera_forward = XMVector3TransformNormal(camera_forward, yRotation);
	}


	//----------------------------------------------------------------------------------
	// Z rotation (Roll)
	//----------------------------------------------------------------------------------
	if (units.z && free_look) {
		XMMATRIX zRotation = XMMatrixRotationAxis(camera_forward, (units.z * turn_factor));
		camera_right = XMVector3TransformNormal(camera_right, zRotation);
		camera_up = XMVector3TransformNormal(camera_up, zRotation);
	}
}


void PlayerCamera::UpdateMovement(float delta_time) {

	XMVECTOR velocity_vec = XMLoadFloat3(&velocity);

	// Limit veloctiy to maximum
	if (XMVectorGetX(XMVector3Length(velocity_vec)) > max_velocity) {

		velocity_vec = XMVector3Normalize(velocity_vec) * max_velocity;
		XMStoreFloat3(&velocity, velocity_vec);
	}


	// Move the camera
	position += camera_right   * velocity.x * delta_time;
	position += camera_up      * velocity.y * delta_time;
	position += camera_forward * velocity.z * delta_time;


	// Update the view matrix and frustum
	UpdateViewMatrix();
	UpdateFrustum();


	// Calculate the new pitch, yaw, and roll values.
	// lookLengthXZ is used in place of camera_forward.z for the pitch. This ensures the
	// pitch does not exceed +-90 degrees, which is needed for limiting the max pitch.
	float3 forward;
	XMStoreFloat3(&forward, camera_forward);

	float lookLengthXZ = sqrtf(powf(forward.x, 2) + powf(forward.z, 2));
	pitch = atan2f(forward.y, lookLengthXZ);
	yaw   = atan2f(forward.x, forward.z);
	roll  = atan2f(forward.x, XMVectorGetY(camera_up));


	// Decelerate the camera
	Decelerate(delta_time);
}


void PlayerCamera::Decelerate(float delta_time) {

	float decel_amount;

	// Decelerate in each direction if not moving in that
	// direction and the current velocity isn't 0.
	if (!is_moving_x && velocity.x != 0.0f) {

		decel_amount = copysign(1.0f, velocity.x) * deceleration * delta_time;

		if (abs(decel_amount) > abs(velocity.x)) {
			velocity.x = 0.0f;
		}
		else {
			velocity.x -= decel_amount;
		}
	}

	if (!is_moving_y && velocity.y != 0.0f) {

		decel_amount = copysign(1.0f, velocity.y) * deceleration * delta_time;

		if (abs(decel_amount) > abs(velocity.y)) {
			velocity.y = 0.0f;
		}
		else {
			velocity.y -= decel_amount;
		}
	}

	if (!is_moving_z && velocity.z != 0.0f) {

		decel_amount = copysign(1.0f, velocity.z) * deceleration * delta_time;

		if (abs(decel_amount) > abs(velocity.z)) {
			velocity.z = 0.0f;
		}
		else {
			velocity.z -= decel_amount;
		}
	}

	// Set is_moving to false. Will be set to true if camera moves again before update.
	is_moving_x = false;
	is_moving_y = false;
	is_moving_z = false;
}