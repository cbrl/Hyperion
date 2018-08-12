#include "camera_motor_system.h"
#include "engine/engine.h"


void CameraMotorSystem::update(Engine& engine) {
	auto& ecs_engine     = engine.getScene().getECS();
	auto& device_context = engine.getRenderingMgr().getDeviceContext();

	const auto process_cam = [&](auto& camera) {
		const auto owner = camera.getOwner();
		auto* entity = ecs_engine.getEntity(owner);

		auto* transform = entity->template getComponent<Transform>();
		if (!transform) return;
		if (!transform->isActive()) return;

		if (auto* movement = entity->template getComponent<CameraMovement>()) {
			processInput(engine, movement, transform);
		}
	};

	ecs_engine.forEach<PerspectiveCamera>([&](PerspectiveCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});


	ecs_engine.forEach<OrthographicCamera>([&](OrthographicCamera& camera) {
		if (camera.isActive())
			process_cam(camera);
	});
}



void CameraMotorSystem::processInput(const Engine& engine, CameraMovement* movement, Transform* transform) const {

	const auto& input = engine.getInput();

	const vec2_i32 mouse_delta = input.getMouseDelta();
	const f32 dt = static_cast<f32>(engine.getTimer().deltaTime());

	vec3_f32 move_units{ 0.0f, 0.0f, 0.0f };


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


	// Move the camera
	updateMovement(movement, move_units);

	move(movement, transform, dt);
}


void CameraMotorSystem::updateMovement(CameraMovement* mv, vec3_f32 units) const {

	// Get the velocity
	vec3_f32 velocity = mv->getVelocity();

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


void CameraMotorSystem::move(CameraMovement* mv, Transform* transform, f32 dt) const {

	const vec3_f32 velocity = mv->getVelocity();
	XMVECTOR velocity_vec = XMLoad(&velocity);
	const f32 velocity_mag = XMVectorGetX(XMVector3Length(velocity_vec));

	// Limit veloctiy to maximum
	if (velocity_mag > mv->getMaxVelocity()) {

		velocity_vec = XMVector3Normalize(velocity_vec) * mv->getMaxVelocity();
		mv->setVelocity(velocity_vec);
	}


	if (velocity_mag != 0.0f) {

		// Move the camera
		XMVECTOR movement = XMVectorZero();

		movement += transform->getWorldAxisX() * mv->getVelocity().x * dt;
		movement += transform->getWorldAxisY() * mv->getVelocity().y * dt;
		movement += transform->getWorldAxisZ() * mv->getVelocity().z * dt;

		transform->move(movement);

		// Decelerate the camera
		decelerate(mv, dt);
	}
}


void CameraMotorSystem::decelerate(CameraMovement* mv, f32 delta_time) const {

	f32 decel_amount;
	vec3_f32 velocity = mv->getVelocity();

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
