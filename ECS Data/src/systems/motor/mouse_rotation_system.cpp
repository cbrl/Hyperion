#include "mouse_rotation_system.h"
#include "engine/engine.h"
#include "components/motor/mouse_rotation.h"
#include "components/transform/transform.h"


void MouseRotationSystem::update(const Engine& engine) {

	auto& ecs_engine        = engine.getECS();
	const auto& input       = engine.getInput();
	const float dt          = static_cast<float>(engine.getTimer().deltaTime());
	const i32_2  mouse_delta = input.getMouseDelta();

	ecs_engine.forEachActive<MouseRotation>([&](MouseRotation& rotation) {

		auto transform = ecs_engine.getComponent<Transform>(rotation.getOwner());
		if (!transform) return;

		f32_3 rotate_units{ 0.0f, 0.0f, 0.0f };

		// Set x/y rotation with mouse data
		rotate_units.x = static_cast<float>(mouse_delta.y) * rotation.sensitivity();
		rotate_units.y = static_cast<float>(mouse_delta.x) * rotation.sensitivity();

		// Rotate the camera
		if (rotate_units.x || rotate_units.y || rotate_units.z) {
			transform->rotate(rotate_units);
		}
	});
}