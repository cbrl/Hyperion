#include "stdafx.h"
#include "camera_system.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\camera\perspective_camera.h"
#include "ecs_data\components\rendering\camera\orthographic_camera.h"


void CameraSystem::Update(float dt) {
	
	ECS::Get()->ForEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		Handle64 owner = camera.GetOwner();
		auto* transform = ECS::Get()->GetComponent<Transform>(owner);

		if (!transform) return;

		if (auto movement = ECS::Get()->GetComponent<CameraMovement>(owner)) {
			ProcessMovement(movement, transform, dt);
		}

		if (transform->Updated()) {
			camera.UpdateViewMatrix(transform->GetPosition(), transform->GetZAxis(), transform->GetYAxis());
		}
	});


	ECS::Get()->ForEach<OrthographicCamera>([&](OrthographicCamera& camera) {
		Handle64 owner = camera.GetOwner();
		auto* transform = ECS::Get()->GetComponent<Transform>(owner);

		if (!transform) return;

		if (auto movement = ECS::Get()->GetComponent<CameraMovement>(owner)) {
			ProcessMovement(movement, transform, dt);
		}

		if (transform->Updated()) {
			camera.UpdateViewMatrix(transform->GetPosition(), transform->GetZAxis(), transform->GetYAxis());
		}
	});
}


void CameraSystem::ProcessMovement(CameraMovement* movement, Transform* transform, float dt) {

	float3 rotate_units(0.0f, 0.0f, 0.0f);
	float3 move_units(0.0f, 0.0f, 0.0f);

	if (auto input = movement->input.lock()) {
		int2 mouse_delta = input->GetMouseDelta();

		// Set x/y rotation with mouse data
		rotate_units.x = XMConvertToRadians(static_cast<float>(mouse_delta.y));
		rotate_units.y = XMConvertToRadians(static_cast<float>(mouse_delta.x));

		// Roll rotation
		if (input->IsKeyDown(Keyboard::Q)) {
			rotate_units.z -= dt;
		}
		else if (input->IsKeyDown(Keyboard::E)) {
			rotate_units.z += dt;
		}

		// Forward/Back movement
		if (input->IsKeyDown(Keyboard::W)) {
			move_units.z += dt;
		}
		else if (input->IsKeyDown(Keyboard::S)) {
			move_units.z -= dt;
		}

		// Left/Right movement
		if (input->IsKeyDown(Keyboard::A)) {
			move_units.x -= dt;
		}
		else if (input->IsKeyDown(Keyboard::D)) {
			move_units.x += dt;
		}

		// Up/Down movement
		if (input->IsKeyDown(Keyboard::Space)) {
			move_units.y += dt;
		}
		else if (input->IsKeyDown(Keyboard::LeftControl)) {
			move_units.y -= dt;
		}
	}


	XMVECTOR camera_forward = transform->GetZAxis();
	XMVECTOR camera_up      = transform->GetYAxis();
	XMVECTOR camera_right   = transform->GetXAxis();

	Move(movement, move_units);
	Rotate(movement, transform, rotate_units, camera_forward, camera_up, camera_right);

	UpdateMovement(movement, transform, camera_forward, camera_up, camera_right, dt);
}


void CameraSystem::Move(CameraMovement* mv, float3 units) {

	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (units.x) {

		// is_moving determines if the camera will decelerate when Update is called
		mv->is_moving_x = true;

		if (copysign(1.0f, units.x) == copysign(1.0f, mv->velocity.x)) {
			mv->velocity.x += units.x * mv->acceleration;
		}
		else {
			mv->velocity.x = units.x * mv->acceleration;
		}
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (units.y) {
		mv->is_moving_y = true;

		if (copysign(1.0f, units.y) == copysign(1.0f, mv->velocity.y)) {
			mv->velocity.y += units.y * mv->acceleration;
		}
		else {
			mv->velocity.y = units.y * mv->acceleration;
		}
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (units.z) {
		mv->is_moving_z = true;

		if (copysign(1.0f, units.z) == copysign(1.0f, mv->velocity.z)) {
			mv->velocity.z += units.z * mv->acceleration;
		}
		else {
			mv->velocity.z = units.z * mv->acceleration;
		}
	}
}


void CameraSystem::Rotate(CameraMovement* mv, Transform* transform, float3 units, XMVECTOR& forward, XMVECTOR& up, XMVECTOR& right) {

	//----------------------------------------------------------------------------------
	// X rotation (Pitch)
	//----------------------------------------------------------------------------------
	if (units.x) {
		float xUnits = units.x * mv->turn_sensitivity;

		// Limit max pitch if free look isn't enabled
		if (!mv->free_look) {
			auto rotation = transform->GetRotation();
			float pitch = XMVectorGetX(rotation);

			pitch -= xUnits;
			//transform->SetRotation(XMVectorSetX(rotation, pitch));

			if (pitch > mv->max_pitch) {
				xUnits += pitch - mv->max_pitch;
			}
			else if (pitch < -mv->max_pitch) {
				xUnits += pitch + mv->max_pitch;
			}
		}

		XMMATRIX xRotation = XMMatrixRotationAxis(right, xUnits);

		// Transform Up vector only if free look is enabled
		if (mv->free_look) {
			up = XMVector3TransformNormal(up, xRotation);
		}
		forward = XMVector3TransformNormal(forward, xRotation);
	}


	//----------------------------------------------------------------------------------
	// Y rotation (Yaw)
	//----------------------------------------------------------------------------------
	if (units.y) {
		XMMATRIX yRotation = XMMatrixRotationAxis(up, (units.y * mv->turn_sensitivity));

		right   = XMVector3TransformNormal(right, yRotation);
		forward = XMVector3TransformNormal(forward, yRotation);
	}


	//----------------------------------------------------------------------------------
	// Z rotation (Roll)
	//----------------------------------------------------------------------------------
	if (units.z && mv->free_look) {
		XMMATRIX zRotation = XMMatrixRotationAxis(forward, (units.z * mv->roll_sensitivity));

		right = XMVector3TransformNormal(right, zRotation);
		up    = XMVector3TransformNormal(up, zRotation);
	}
}


void CameraSystem::UpdateMovement(CameraMovement* mv, Transform* transform, XMVECTOR& forward, XMVECTOR& up, XMVECTOR& right, float dt) {

	XMVECTOR velocity_vec = XMLoadFloat3(&mv->velocity);

	// Limit veloctiy to maximum
	if (XMVectorGetX(XMVector3Length(velocity_vec)) > mv->max_velocity) {

		velocity_vec = XMVector3Normalize(velocity_vec) * mv->max_velocity;
		XMStoreFloat3(&mv->velocity, velocity_vec);
	}


	// Move the camera
	XMVECTOR position = transform->GetPosition();

	position += right   * mv->velocity.x * dt;
	position += up      * mv->velocity.y * dt;
	position += forward * mv->velocity.z * dt;

	transform->SetPosition(position);


	// Calculate the new pitch, yaw, and roll values.
	// lookLengthXZ is used in place of camera_forward.z for the pitch. This ensures the
	// pitch does not exceed +-90 degrees, which is needed for limiting the max pitch.
	float3 forward_f32;
	XMStoreFloat3(&forward_f32, forward);

	float3 rotation(0.0f, 0.0f, 0.0f);

	float lookLengthXZ = sqrtf(powf(forward_f32.x, 2) + powf(forward_f32.z, 2));
	rotation.x = atan2f(forward_f32.y, lookLengthXZ);
	rotation.y = atan2f(forward_f32.x, forward_f32.z);
	rotation.z = atan2f(forward_f32.x, XMVectorGetY(up));

	transform->SetRotation(rotation);


	// Decelerate the camera
	Decelerate(mv, dt);
}


void CameraSystem::Decelerate(CameraMovement* mv, float delta_time) {

	float decel_amount;

	// Decelerate in each direction if not moving in that
	// direction and the current velocity isn't 0.
	if (!mv->is_moving_x && mv->velocity.x != 0.0f) {

		decel_amount = copysign(1.0f, mv->velocity.x) * mv->deceleration * delta_time;

		if (abs(decel_amount) > abs(mv->velocity.x)) {
			mv->velocity.x = 0.0f;
		}
		else {
			mv->velocity.x -= decel_amount;
		}
	}

	if (!mv->is_moving_y && mv->velocity.y != 0.0f) {

		decel_amount = copysign(1.0f, mv->velocity.y) * mv->deceleration * delta_time;

		if (abs(decel_amount) > abs(mv->velocity.y)) {
			mv->velocity.y = 0.0f;
		}
		else {
			mv->velocity.y -= decel_amount;
		}
	}

	if (!mv->is_moving_z && mv->velocity.z != 0.0f) {

		decel_amount = copysign(1.0f, mv->velocity.z) * mv->deceleration * delta_time;

		if (abs(decel_amount) > abs(mv->velocity.z)) {
			mv->velocity.z = 0.0f;
		}
		else {
			mv->velocity.z -= decel_amount;
		}
	}

	// Set is_moving to false. Will be set to true if camera moves again before update.
	mv->is_moving_x = false;
	mv->is_moving_y = false;
	mv->is_moving_z = false;
}