#include "stdafx.h"
#include "camera_system.h"
#include "engine/engine.h"
#include "ecs_data/components/rendering/camera/perspective_camera.h"
#include "ecs_data/components/rendering/camera/orthographic_camera.h"


void CameraSystem::update(const Engine& engine) {
	auto& ecs_engine     = engine.getECS();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	ecs_engine.forEachActive<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		const handle64 owner = camera.getOwner();
		const auto transform = ecs_engine.getComponent<CameraTransform>(owner);

		if (!transform) return;


		// Process movement
		if (const auto movement = ecs_engine.getComponent<CameraMovement>(owner)) {
			processMovement(engine, movement, transform);
		}


		// Update the camera's view matrix
		camera.updateViewMatrix(transform->getPosition(), transform->getWorldAxisZ(), transform->getWorldAxisY());


		// Update the camera's buffer
		camera.updateBuffer(device_context,
		                    transform->getWorldOrigin(),
		                    transform->getWorldToObjectMatrix(),
		                    camera.getProjectionMatrix());
	});


	ecs_engine.forEachActive<OrthographicCamera>([&](OrthographicCamera& camera) {
		const handle64 owner = camera.getOwner();
		const auto transform = ecs_engine.getComponent<CameraTransform>(owner);

		if (!transform) return;


		// Process movement
		if (const auto movement = ecs_engine.getComponent<CameraMovement>(owner)) {
			processMovement(engine, movement, transform);
		}


		// Update the camera's view matrix
		camera.updateViewMatrix(transform->getPosition(), transform->getWorldAxisZ(), transform->getWorldAxisY());


		// Update the camera's buffer
		camera.updateBuffer(device_context,
		                    transform->getWorldOrigin(),
		                    transform->getWorldToObjectMatrix(),
		                    camera.getProjectionMatrix());
	});
}


void CameraSystem::processMovement(const Engine& engine, CameraMovement* movement, CameraTransform* transform) const {

	const auto& input = engine.getInput();

	const int2 mouse_delta = input.getMouseDelta();
	const float dt         = engine.getTimer().deltaTime();

	float3 rotate_units(0.0f, 0.0f, 0.0f);
	float3 move_units(0.0f, 0.0f, 0.0f);


	//----------------------------------------------------------------------------------
	// Rotation
	//----------------------------------------------------------------------------------

	{
		// Set x/y rotation with mouse data
		rotate_units.x = XMConvertToRadians(static_cast<float>(mouse_delta.y)) * movement->getTurnSensitivity();
		rotate_units.y = XMConvertToRadians(static_cast<float>(mouse_delta.x)) * movement->getTurnSensitivity();

		// Roll rotation
		if (input.isKeyDown(Keyboard::Q)) {
			rotate_units.z += dt * movement->getRollSensitivity();
		}
		else if (input.isKeyDown(Keyboard::E)) {
			rotate_units.z -= dt * movement->getRollSensitivity();
		}
	}

	// Rotate the camera
	if (rotate_units.x || rotate_units.y || rotate_units.z) {
		transform->rotate(rotate_units);
	}


	//----------------------------------------------------------------------------------
	// Movement
	//----------------------------------------------------------------------------------

	{
		// Forward/Back movement
		if (input.isKeyDown(Keyboard::W)) {
			move_units.z += dt;
		}
		else if (input.isKeyDown(Keyboard::S)) {
			move_units.z -= dt;
		}

		// Left/Right movement
		if (input.isKeyDown(Keyboard::A)) {
			move_units.x -= dt;
		}
		else if (input.isKeyDown(Keyboard::D)) {
			move_units.x += dt;
		}

		// Up/Down movement
		if (input.isKeyDown(Keyboard::Space)) {
			move_units.y += dt;
		}
		else if (input.isKeyDown(Keyboard::LeftControl)) {
			move_units.y -= dt;
		}
	}


	// Move the camera
	updateMovement(movement, move_units);

	move(movement, transform, dt);
}


void CameraSystem::updateMovement(CameraMovement* mv, float3 units) const {

	// Get the velocity
	float3 velocity = mv->getVelocity();

	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (units.x) {

		// is_moving determines if the camera will decelerate when Update is called
		mv->setMovingX(true);

		if (copysign(1.0f, units.x) == copysign(1.0f, velocity.x)) {
			velocity.x += units.x * mv->getAcceleration();
		}
		else {
			velocity.x = units.x * mv->getAcceleration();
		}
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (units.y) {
		mv->setMovingY(true);

		if (copysign(1.0f, units.y) == copysign(1.0f, velocity.y)) {
			velocity.y += units.y * mv->getAcceleration();
		}
		else {
			velocity.y = units.y * mv->getAcceleration();
		}
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (units.z) {
		mv->setMovingZ(true);

		if (copysign(1.0f, units.z) == copysign(1.0f, velocity.z)) {
			velocity.z += units.z * mv->getAcceleration();
		}
		else {
			velocity.z = units.z * mv->getAcceleration();
		}
	}

	// Set the new velocity
	mv->setVelocity(velocity);
}


void CameraSystem::move(CameraMovement* mv, CameraTransform* transform, float dt) const {

	const float3 velocity    = mv->getVelocity();
	XMVECTOR velocity_vec    = XMLoadFloat3(&velocity);
	const float velocity_mag = XMVectorGetX(XMVector3Length(velocity_vec));

	// Limit veloctiy to maximum
	if (velocity_mag > mv->getMaxVelocity()) {

		velocity_vec = XMVector3Normalize(velocity_vec) * mv->getMaxVelocity();
		mv->setVelocity(velocity_vec);
	}


	if (velocity_mag != 0.0f) {

		// Move the camera
		XMVECTOR position = XMVectorZero();

		position += transform->getWorldAxisX() * mv->getVelocity().x * dt;
		position += transform->getWorldAxisY() * mv->getVelocity().y * dt;
		position += transform->getWorldAxisZ() * mv->getVelocity().z * dt;

		transform->move(position);

		// Decelerate the camera
		decelerate(mv, dt);
	}
}


void CameraSystem::decelerate(CameraMovement* mv, float delta_time) const {

	float decel_amount;
	float3 velocity = mv->getVelocity();

	// Decelerate in each direction if not moving in that
	// direction and the current velocity isn't 0.
	if (!mv->isMovingX() && velocity.x != 0.0f) {

		decel_amount = copysign(1.0f, velocity.x) * mv->getDeceleration() * delta_time;

		if (abs(decel_amount) > abs(velocity.x)) {
			velocity.x = 0.0f;
		}
		else {
			velocity.x -= decel_amount;
		}
	}

	if (!mv->isMovingY() && velocity.y != 0.0f) {

		decel_amount = copysign(1.0f, velocity.y) * mv->getDeceleration() * delta_time;

		if (abs(decel_amount) > abs(velocity.y)) {
			velocity.y = 0.0f;
		}
		else {
			velocity.y -= decel_amount;
		}
	}

	if (!mv->isMovingZ() && velocity.z != 0.0f) {

		decel_amount = copysign(1.0f, velocity.z) * mv->getDeceleration() * delta_time;

		if (abs(decel_amount) > abs(velocity.z)) {
			velocity.z = 0.0f;
		}
		else {
			velocity.z -= decel_amount;
		}
	}

	// Set the new velocity
	mv->setVelocity(velocity);

	// Set is_moving to false. Will be set to true if camera moves again before update.
	mv->setMovingX(false);
	mv->setMovingY(false);
	mv->setMovingZ(false);
}
