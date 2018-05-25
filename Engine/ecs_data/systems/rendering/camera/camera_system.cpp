#include "stdafx.h"
#include "camera_system.h"
#include "engine\engine.h"
#include "ecs_data\components\rendering\camera\perspective_camera.h"
#include "ecs_data\components\rendering\camera\orthographic_camera.h"


void CameraSystem::Update(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();

	ecs_engine.ForEachActive<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		Handle64 owner = camera.GetOwner();
		const auto transform = ecs_engine.GetComponent<CameraTransform>(owner);

		if (!transform) return;

		if (auto movement = ecs_engine.GetComponent<CameraMovement>(owner)) {
			ProcessMovement(engine, movement, transform);
		}

		camera.UpdateViewMatrix(transform->GetPosition(), transform->GetWorldAxisZ(), transform->GetWorldAxisY());
	});


	ecs_engine.ForEachActive<OrthographicCamera>([&](OrthographicCamera& camera) {
		Handle64 owner = camera.GetOwner();
		const auto transform = ecs_engine.GetComponent<CameraTransform>(owner);

		if (!transform) return;

		if (auto movement = ecs_engine.GetComponent<CameraMovement>(owner)) {
			ProcessMovement(engine, movement, transform);
		}

		camera.UpdateViewMatrix(transform->GetPosition(), transform->GetWorldAxisZ(), transform->GetWorldAxisY());
	});
}


void CameraSystem::ProcessMovement(const Engine& engine, CameraMovement* movement, CameraTransform* transform) {

	const auto& input = engine.GetInput();

	int2  mouse_delta = input.GetMouseDelta();
	float dt = engine.GetTimer().DeltaTime();

	float3 rotate_units(0.0f, 0.0f, 0.0f);
	float3 move_units(0.0f, 0.0f, 0.0f);


	//----------------------------------------------------------------------------------
	// Rotation
	//----------------------------------------------------------------------------------

	{
		// Set x/y rotation with mouse data
		rotate_units.x = XMConvertToRadians(static_cast<float>(mouse_delta.y)) * movement->GetTurnSensitivity();
		rotate_units.y = XMConvertToRadians(static_cast<float>(mouse_delta.x)) * movement->GetTurnSensitivity();

		// Roll rotation
		if (input.IsKeyDown(Keyboard::Q)) {
			rotate_units.z += dt * movement->GetRollSensitivity();
		}
		else if (input.IsKeyDown(Keyboard::E)) {
			rotate_units.z -= dt * movement->GetRollSensitivity();
		}
	}

	// Rotate the camera
	if (rotate_units.x || rotate_units.y || rotate_units.z) {
		transform->Rotate(rotate_units);
	}


	//----------------------------------------------------------------------------------
	// Movement
	//----------------------------------------------------------------------------------

	{
		// Forward/Back movement
		if (input.IsKeyDown(Keyboard::W)) {
			move_units.z += dt;
		}
		else if (input.IsKeyDown(Keyboard::S)) {
			move_units.z -= dt;
		}

		// Left/Right movement
		if (input.IsKeyDown(Keyboard::A)) {
			move_units.x -= dt;
		}
		else if (input.IsKeyDown(Keyboard::D)) {
			move_units.x += dt;
		}

		// Up/Down movement
		if (input.IsKeyDown(Keyboard::Space)) {
			move_units.y += dt;
		}
		else if (input.IsKeyDown(Keyboard::LeftControl)) {
			move_units.y -= dt;
		}
	}


	// Move the camera
	UpdateMovement(movement, move_units);

	Move(movement, transform, dt);
}


void CameraSystem::UpdateMovement(CameraMovement* mv, float3 units) {

	// Get the velocity
	float3 velocity = mv->GetVelocity();

	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (units.x) {

		// is_moving determines if the camera will decelerate when Update is called
		mv->SetMovingX(true);

		if (copysign(1.0f, units.x) == copysign(1.0f, velocity.x)) {
			velocity.x += units.x * mv->GetAcceleration();
		}
		else {
			velocity.x = units.x * mv->GetAcceleration();
		}
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (units.y) {
		mv->SetMovingY(true);

		if (copysign(1.0f, units.y) == copysign(1.0f, velocity.y)) {
			velocity.y += units.y * mv->GetAcceleration();
		}
		else {
			velocity.y = units.y * mv->GetAcceleration();
		}
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (units.z) {
		mv->SetMovingZ(true);

		if (copysign(1.0f, units.z) == copysign(1.0f, velocity.z)) {
			velocity.z += units.z * mv->GetAcceleration();
		}
		else {
			velocity.z = units.z * mv->GetAcceleration();
		}
	}

	// Set the new velocity
	mv->SetVelocity(velocity);
}


void CameraSystem::Move(CameraMovement* mv, CameraTransform* transform, float dt) {

	XMVECTOR velocity_vec = XMLoadFloat3(&mv->GetVelocity());
	float velocity_mag    = XMVectorGetX(XMVector3Length(velocity_vec));

	// Limit veloctiy to maximum
	if (velocity_mag > mv->GetMaxVelocity()) {

		velocity_vec = XMVector3Normalize(velocity_vec) * mv->GetMaxVelocity();
		mv->SetVelocity(velocity_vec);
	}


	if (velocity_mag != 0.0f) {

		// Move the camera
		XMVECTOR position = XMVectorZero();

		position += transform->GetWorldAxisX() * mv->GetVelocity().x * dt;
		position += transform->GetWorldAxisY() * mv->GetVelocity().y * dt;
		position += transform->GetWorldAxisZ() * mv->GetVelocity().z * dt;

		transform->Move(position);

		// Decelerate the camera
		Decelerate(mv, dt);
	}
}


void CameraSystem::Decelerate(CameraMovement* mv, float delta_time) {

	float decel_amount;
	float3 velocity = mv->GetVelocity();

	// Decelerate in each direction if not moving in that
	// direction and the current velocity isn't 0.
	if (!mv->IsMovingX() && velocity.x != 0.0f) {

		decel_amount = copysign(1.0f, velocity.x) * mv->GetDeceleration() * delta_time;

		if (abs(decel_amount) > abs(velocity.x)) {
			velocity.x = 0.0f;
		}
		else {
			velocity.x -= decel_amount;
		}
	}

	if (!mv->IsMovingY() && velocity.y != 0.0f) {

		decel_amount = copysign(1.0f, velocity.y) * mv->GetDeceleration() * delta_time;

		if (abs(decel_amount) > abs(velocity.y)) {
			velocity.y = 0.0f;
		}
		else {
			velocity.y -= decel_amount;
		}
	}

	if (!mv->IsMovingZ() && velocity.z != 0.0f) {

		decel_amount = copysign(1.0f, velocity.z) * mv->GetDeceleration() * delta_time;

		if (abs(decel_amount) > abs(velocity.z)) {
			velocity.z = 0.0f;
		}
		else {
			velocity.z -= decel_amount;
		}
	}

	// Set the new velocity
	mv->SetVelocity(velocity);

	// Set is_moving to false. Will be set to true if camera moves again before update.
	mv->SetMovingX(false);
	mv->SetMovingY(false);
	mv->SetMovingZ(false);
}