#include "mouse_rotation_system.h"

#include "engine/engine.h"
#include "scene/components/transform/transform.h"
#include "scenes/test_scene/components/motor/mouse_rotation.h"


MouseRotationSystem::MouseRotationSystem(const Input& input)
	:input(input) {
}


void MouseRotationSystem::update() {

	const i32_2 mouse_delta = input.getMouseDelta();

	getECS().forEach<Transform, MouseRotation>([&](ecs::Entity& entity) {
		auto& transform      = entity.getComponent<Transform>();
		const auto& rotation = entity.getComponent<MouseRotation>();

		if (!rotation.isActive())
			return;

		const f32_2 max = rotation.getMaxRotation();
		f32_2 units{0.0f, 0.0f};

		// Set x/y rotation with mouse data
		units[0] = static_cast<f32>(mouse_delta[1]) * rotation.getSensitivity();
		units[1] = static_cast<f32>(mouse_delta[0]) * rotation.getSensitivity();

		// Rotate the camera
		if (units[0]) {
			transform.rotateXClamped(units[0], -max[0], max[0]);
		}
		if (units[1]) { 
			transform.rotateYClamped(units[1], -max[1], max[1]);
		}
	});
}