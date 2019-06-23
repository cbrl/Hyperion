#include "camera_motor_system.h"
#include "engine/engine.h"
#include "scenes/test_scene/components/motor/camera_movement.h"


CameraMotorSystem::CameraMotorSystem(const Input& input, KeyConfig& key_config)
	: input(input)
	, key_config(key_config) {

	key_config.bindIfNotBound("Forward", Keyboard::W);
	key_config.bindIfNotBound("Back",    Keyboard::S);
	key_config.bindIfNotBound("Left",    Keyboard::A);
	key_config.bindIfNotBound("Right",   Keyboard::D);
	key_config.bindIfNotBound("Up",      Keyboard::Space);
	key_config.bindIfNotBound("Down",    Keyboard::LeftControl);
}


void CameraMotorSystem::update() {

	getECS().forEach<Transform, PerspectiveCamera, CameraMovement>([&](ecs::Entity& entity) {
		auto& transform = entity.getComponent<Transform>();
		auto& camera    = entity.getComponent<PerspectiveCamera>();
		auto& movement  = entity.getComponent<CameraMovement>();

		if (camera.isActive() && transform.isActive() && movement.isActive()) {
			processInput(movement, transform);
		}
	});

	getECS().forEach<Transform, OrthographicCamera, CameraMovement>([&](ecs::Entity& entity) {
		auto& transform = entity.getComponent<Transform>();
		auto& camera    = entity.getComponent<OrthographicCamera>();
		auto& movement  = entity.getComponent<CameraMovement>();

		if (camera.isActive() && transform.isActive() && movement.isActive()) {
			processInput(movement, transform);
		}
	});
}


void CameraMotorSystem::registerCallbacks() {
	registerEventCallback(&CameraMotorSystem::onGuiFocus);
}


void CameraMotorSystem::onGuiFocus(const render::events::GuiFocusEvent& event) {
	this->setActive(!event.keyboard_focus);
}


void CameraMotorSystem::processInput(CameraMovement& movement, Transform& transform) const {

	const auto dt = static_cast<f32>(dtSinceLastUpdate().count());
	f32_3 move_units = { 0.0f, 0.0f, 0.0f };

	// Forward/Back movement
	if (input.isKeyDown(key_config.getKey("Forward"))) {
		move_units[2] += dt;
	}
	else if (input.isKeyDown(key_config.getKey("Back"))) {
		move_units[2] -= dt;
	}

	// Left/Right movement
	if (input.isKeyDown(key_config.getKey("Left"))) {
		move_units[0] -= dt;
	}
	else if (input.isKeyDown(key_config.getKey("Right"))) {
		move_units[0] += dt;
	}

	// Up/Down movement
	if (input.isKeyDown(key_config.getKey("Up"))) {
		move_units[1] += dt;
	}
	else if (input.isKeyDown(key_config.getKey("Down"))) {
		move_units[1] -= dt;
	}


	// Move the camera
	updateMovement(movement, move_units);

	move(movement, transform);
}


void CameraMotorSystem::updateMovement(CameraMovement& mv, f32_3 units) const {

	// Get the velocity
	f32_3 velocity = mv.getVelocity();

	//----------------------------------------------------------------------------------
	// X movement
	//----------------------------------------------------------------------------------
	// If the movement is in the same direction as the camera is currently moving,
	// then add to the velocity. Otherwise, reset the velocity using the new value.
	if (units[0]) {

		// is_moving determines if the camera will decelerate when Update is called
		mv.setMovingX(true);

		if (copysign(1.0f, units[0]) == copysign(1.0f, velocity[0])) {
			velocity[0] += units[0] * mv.getAcceleration();
		}
		else {
			velocity[0] = units[0] * mv.getAcceleration();
		}
	}


	//----------------------------------------------------------------------------------
	// Y movement
	//----------------------------------------------------------------------------------
	if (units[1]) {
		mv.setMovingY(true);

		if (copysign(1.0f, units[1]) == copysign(1.0f, velocity[1])) {
			velocity[1] += units[1] * mv.getAcceleration();
		}
		else {
			velocity[1] = units[1] * mv.getAcceleration();
		}
	}


	//----------------------------------------------------------------------------------
	// Z movement
	//----------------------------------------------------------------------------------
	if (units[2]) {
		mv.setMovingZ(true);

		if (copysign(1.0f, units[2]) == copysign(1.0f, velocity[2])) {
			velocity[2] += units[2] * mv.getAcceleration();
		}
		else {
			velocity[2] = units[2] * mv.getAcceleration();
		}
	}

	// Set the new velocity
	mv.setVelocity(velocity);
}


void CameraMotorSystem::move(CameraMovement& mv, Transform& transform) const {

	const f32_3 velocity = mv.getVelocity();
	XMVECTOR velocity_vec = XMLoad(&velocity);
	const f32 velocity_mag = XMVectorGetX(XMVector3Length(velocity_vec));

	// Limit veloctiy to maximum
	if (velocity_mag > mv.getMaxVelocity()) {
		velocity_vec = XMVector3Normalize(velocity_vec) * mv.getMaxVelocity();
		mv.setVelocity(velocity_vec);
	}


	if (velocity_mag != 0.0f) {
		const auto dt = static_cast<f32>(dtSinceLastUpdate().count());

		// Move the camera
		XMVECTOR movement = XMVectorZero();
		movement += transform.getWorldAxisX() * mv.getVelocity()[0] * dt;
		movement += transform.getWorldAxisY() * mv.getVelocity()[1] * dt;
		movement += transform.getWorldAxisZ() * mv.getVelocity()[2] * dt;

		transform.move(movement);

		// Decelerate the camera
		decelerate(mv);
	}
}


void CameraMotorSystem::decelerate(CameraMovement& mv) const {

	f32_3 velocity = mv.getVelocity();
	const auto dt = static_cast<f32>(dtSinceLastUpdate().count());

	// Decelerate in each direction if not moving in that
	// direction and the current velocity isn't 0.
	if (!mv.isMovingX() && velocity[0] != 0.0f) {
		const f32 decel_amount = copysign(1.0f, velocity[0]) * mv.getDeceleration() * dt;

		if (abs(decel_amount) > abs(velocity[0])) {
			velocity[0] = 0.0f;
		}
		else {
			velocity[0] -= decel_amount;
		}
	}

	if (!mv.isMovingY() && velocity[1] != 0.0f) {
		const f32 decel_amount = copysign(1.0f, velocity[1]) * mv.getDeceleration() * dt;

		if (abs(decel_amount) > abs(velocity[1])) {
			velocity[1] = 0.0f;
		}
		else {
			velocity[1] -= decel_amount;
		}
	}

	if (!mv.isMovingZ() && velocity[2] != 0.0f) {
		const f32 decel_amount = copysign(1.0f, velocity[2]) * mv.getDeceleration() * dt;

		if (abs(decel_amount) > abs(velocity[2])) {
			velocity[2] = 0.0f;
		}
		else {
			velocity[2] -= decel_amount;
		}
	}

	// Set the new velocity
	mv.setVelocity(velocity);

	// Set is_moving to false. Will be set to true if camera moves again before update.
	mv.setMovingX(false);
	mv.setMovingY(false);
	mv.setMovingZ(false);
}
