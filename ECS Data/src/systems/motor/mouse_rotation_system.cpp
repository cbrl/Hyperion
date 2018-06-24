#include "mouse_rotation_system.h"
#include "engine/engine.h"
#include "components/motor/mouse_rotation.h"
#include "components/transform/transform.h"


void MouseRotationSystem::update(const Engine& engine) {

	auto& ecs_engine  = engine.getECS();
	const auto& input = engine.getInput();

	const f32 dt = static_cast<f32>(engine.getTimer().deltaTime());
	const vec2_i32 mouse_delta = input.getMouseDelta();

	ecs_engine.forEachActive<MouseRotation>([&](MouseRotation& rotation) {

		auto transform = ecs_engine.getComponent<Transform>(rotation.getOwner());
		if (!transform) return;

		const vec2_f32 max = rotation.getMaxRotation();
		vec2_f32 units{ 0.0f, 0.0f };

		// Set x/y rotation with mouse data
		units.x = static_cast<f32>(mouse_delta.y) * rotation.getSensitivity();
		units.y = static_cast<f32>(mouse_delta.x) * rotation.getSensitivity();

		// Rotate the camera
		if (units.x) {
			transform->rotateXClamped(units.x, -max.x, max.x);
		}
		if (units.y) { 
			transform->rotateYClamped(units.y, -max.y, max.y);
		}
	});
}