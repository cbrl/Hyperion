#include "mouse_rotation_system.h"

#include "engine/engine.h"
#include "components/motor/mouse_rotation.h"
#include "scene/components/transform/transform.h"


void MouseRotationSystem::update(Engine& engine) {

	auto& scene       = engine.getScene();
	const auto& input = engine.getInput();

	const vec2_i32 mouse_delta = input.getMouseDelta();

	scene.forEach<MouseRotation>([&](MouseRotation& rotation) {

		if (!rotation.isActive()) return;

		auto* transform = rotation.getOwner()->getComponent<Transform>();
		if (!transform) return;
		if (!transform->isActive()) return;

		const vec2_f32 max = rotation.getMaxRotation();
		vec2_f32 units{0.0f, 0.0f};

		// Set x/y rotation with mouse data
		units[0] = static_cast<f32>(mouse_delta[1]) * rotation.getSensitivity();
		units[1] = static_cast<f32>(mouse_delta[0]) * rotation.getSensitivity();

		// Rotate the camera
		if (units[0]) {
			transform->rotateXClamped(units[0], -max[0], max[0]);
		}
		if (units[1]) { 
			transform->rotateYClamped(units[1], -max[1], max[1]);
		}
	});
}