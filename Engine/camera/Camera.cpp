#include "stdafx.h"
#include "camera.h"
#include "util\math\math.h"


Camera::Camera()
	: enable_free_look(false)
	, fps_mode(false)

	, look_at(XMVectorZero())
	, position(XMVectorZero())
	, velocity(0.0f, 0.0f, 0.0f)
	, move_accel(0.00002f)
	, move_decel(0.00005f)
	, max_velocity(0.01f)
	, is_moving_x(false)
	, is_moving_y(false)
	, is_moving_z(false)

	, turn_factor(0.002f)
	, pitch(0.0f)
	, yaw(0.0f)
	, roll(0.0f)
	, max_pitch(XMConvertToRadians(89.0f))

	, camera_forward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))
	, fps_forward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))
	, camera_right(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f))
	, camera_up(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))

	, fov(XM_PI/4.0f)
	, viewport_width(800)
	, viewport_height(800)
	, z_near(0.1f)
	, z_far(1000.0f)
	, view_matrix(XMMatrixIdentity())
	, projection_matrix(XMMatrixIdentity())
	, ortho_matrix(XMMatrixIdentity())
{
	if (enable_free_look) fps_mode = false;
}


Camera::Camera(u32 viewportWidth, u32 viewportHeight,
			   float FOV, float zNear, float zFar) : Camera()
{
	viewport_width = viewportWidth;
	viewport_height = viewportHeight;
	fov = FOV;
	z_near = zNear;
	z_far = zFar;

	aspect_ratio      = (float)viewport_width / (float)viewport_height;
	projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);

	ortho_matrix = XMMatrixOrthographicLH((float)viewport_width, (float)viewport_height, z_near, z_far);
}


void Camera::Move(float3 units) {
	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (units.x) {

		// is_moving determines if the camera will decelerate when Update is called
		is_moving_x = true;

		if (copysign(1.0f, units.x) == copysign(1.0f, velocity.x)) {
			velocity.x += units.x * move_accel;
		}
		else {
			velocity.x = units.x * move_accel;
		}
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (units.y) {
		is_moving_y = true;

		if (copysign(1.0f, units.y) == copysign(1.0f, velocity.y)) {
			velocity.y += units.y * move_accel;
		}
		else {
			velocity.y = units.y * move_accel;
		}
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (units.z) {
		is_moving_z = true;

		if (copysign(1.0f, units.z) == copysign(1.0f, velocity.z)) {
			velocity.z += units.z * move_accel;
		}
		else {
			velocity.z = units.z * move_accel;
		}
	}
}


void Camera::Rotate(float3 units) {
	//----------------------------------------------------------------------------------
	// X rotation (Pitch)
	//----------------------------------------------------------------------------------
	if (units.x) {
		float xUnits = units.x * turn_factor;

		// Limit max pitch if free look isn't enabled
		if (!enable_free_look) {
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
		if (enable_free_look) {
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

		// fps_forward will be used for movement if fps_mode is enabled
		if (fps_mode) {
			fps_forward = XMVector3TransformNormal(fps_forward, yRotation);
		}
	}


	//----------------------------------------------------------------------------------
	// Z rotation (Roll)
	//----------------------------------------------------------------------------------
	if (units.z && enable_free_look) {
		XMMATRIX zRotation = XMMatrixRotationAxis(camera_forward, (units.z * turn_factor));
		camera_right = XMVector3TransformNormal(camera_right, zRotation);
		camera_up = XMVector3TransformNormal(camera_up, zRotation);
	}
}


void Camera::Update(float delta_time) {

	XMVECTOR velocityVec = XMLoadFloat3(&velocity);

	// Limit veloctiy to maximum
	if (XMVectorGetX(XMVector3Length(velocityVec)) > max_velocity) {

		velocityVec = XMVector3Normalize(velocityVec) * max_velocity;
		XMStoreFloat3(&velocity, velocityVec);
	}


	// Move camera
	position += camera_right * velocity.x * delta_time;
	position += camera_up    * velocity.y * delta_time;
	if (fps_mode) {
		position += fps_forward * velocity.z * delta_time;
	}
	else {
		position += camera_forward * velocity.z * delta_time;
	}


	// Add the position and forward vectors of the camera to the target vector
	look_at = position + camera_forward;


	// Create the new view matrix
	view_matrix = XMMatrixLookAtLH(position, look_at, camera_up);


	// Calculate the new pitch, yaw, and roll values.
	// lookLengthXZ is used in place of camera_forward.z for the pitch. This ensures the
	// pitch does not exceed +-90 degrees, which is needed for limiting the max pitch.
	float lookLengthXZ = sqrtf(powf(XMVectorGetX(camera_forward), 2) + powf(XMVectorGetZ(camera_forward), 2));
	pitch = atan2f(XMVectorGetY(camera_forward), lookLengthXZ);
	yaw   = atan2f(XMVectorGetX(camera_forward), XMVectorGetZ(camera_forward));
	roll  = atan2f(XMVectorGetX(camera_up), XMVectorGetY(camera_up));


	// Decelerate if not moving
	float deceleration;

	if (!is_moving_x) {
		if (velocity.x != 0.0f) {
			deceleration = copysign(1.0f, velocity.x) * move_decel * delta_time;

			if (abs(deceleration) > abs(velocity.x)) {
				velocity.x = 0.0f;
			}
			else {
				velocity.x -= deceleration;
			}
		}
	}

	if (!is_moving_y) {
		if (velocity.y != 0.0f) {
			deceleration = copysign(1.0f, velocity.y) * move_decel * delta_time;

			if (abs(deceleration) > abs(velocity.y)) {
				velocity.y = 0.0f;
			}
			else {
				velocity.y -= deceleration;
			}
		}
	}

	if (!is_moving_z) {
		if (velocity.z != 0.0f) {
			deceleration = copysign(1.0f, velocity.z) * move_decel * delta_time;

			if (abs(deceleration) > abs(velocity.z)) {
				velocity.z = 0.0f;
			}
			else {
				velocity.z -= deceleration;
			}
		}
	}

	// Set IsMoving to false. Will be set to true if camera moves again before update.
	is_moving_x = false;
	is_moving_y = false;
	is_moving_z = false;
}
