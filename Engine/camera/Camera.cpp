#include "stdafx.h"
#include "camera.h"
#include "util\math\math.h"


Camera::Camera(ID3D11Device* device)
	: buffer(device)
	
	, free_look(false)

	, look_at(XMVectorZero())
	, position(XMVectorZero())
	, velocity(0.0f, 0.0f, 0.0f)
	, acceleration(0.00002f)
	, deceleration(0.00005f)
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
{}


Camera::Camera(ID3D11Device* device,
			   ResourceMgr& resource_mgr,
			   u32 viewportWidth,
			   u32 viewportHeight,
			   float FOV,
			   float zNear,
			   float zFar,
			   wstring skybox_filename)
	: Camera(device)
{
	viewport_width  = viewportWidth;
	viewport_height = viewportHeight;
	fov             = FOV;
	z_near          = zNear;
	z_far           = zFar;


	// Calculate the aspect ratio
	aspect_ratio      = (float)viewport_width / (float)viewport_height;


	// Calculate the projection and orthographic matrices
	projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
	ortho_matrix      = XMMatrixOrthographicLH((float)viewport_width, (float)viewport_height, z_near, z_far);


	// Initialize the camera's skybox
	skybox.Init(device, resource_mgr, skybox_filename);
}


void Camera::OnResize(u32 width, u32 height) {

	// Set the new veiwport width and height
	viewport_width = width;
	viewport_height = height;


	// Recalculate the aspect ratio
	aspect_ratio = (float)viewport_width / (float)viewport_height;


	// Recalculate the projection and orthographic matrices
	projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
	ortho_matrix      = XMMatrixOrthographicLH((float)viewport_width, (float)viewport_height, z_near, z_far);
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


void Camera::Rotate(float3 units) {
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


void Camera::UpdateMovement(float delta_time) {

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


	// Update the target vector with sum of the position and forward vectors
	look_at = position + camera_forward;

	// Create the new view matrix
	view_matrix = XMMatrixLookAtLH(position, look_at, camera_up);

	// Update the frustum
	frustum.UpdateFrustum(view_matrix * projection_matrix);


	// Calculate the new pitch, yaw, and roll values.
	// lookLengthXZ is used in place of camera_forward.z for the pitch. This ensures the
	// pitch does not exceed +-90 degrees, which is needed for limiting the max pitch.
	float3 forward;
	XMStoreFloat3(&forward, camera_forward);

	float lookLengthXZ = sqrtf(powf(forward.x, 2) + powf(forward.z, 2));
	pitch = atan2f(forward.y, lookLengthXZ);
	yaw   = atan2f(forward.x, forward.z);
	roll  = atan2f(forward.x, XMVectorGetY(camera_up));


	//----------------------------------------------------------------------------------
	// Decelerate if not moving
	//----------------------------------------------------------------------------------

	float decel_amount;

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
