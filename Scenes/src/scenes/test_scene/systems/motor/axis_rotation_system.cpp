#include "axis_rotation_system.h"

#include "engine/engine.h"
#include "scene/components/transform/transform.h"
#include "scenes/test_scene/components/motor/axis_rotation.h"


void AxisRotationSystem::update() {

	getECS().forEach<Transform, AxisRotation>([&](ecs::Entity& entity) {
		auto& transform      = *entity.getComponent<Transform>();
		const auto& rotation = *entity.getComponent<AxisRotation>();

		if (!rotation.isActive() || !transform.isActive())
			return;

		const auto dt = static_cast<f32>(dtSinceLastUpdate().count());

		if (rotation.hasAxis(AxisRotation::Axis::X)) {
			transform.rotateXClamped(dt * rotation.getSpeedX(), -XM_PI, XM_PI);
		}
		if (rotation.hasAxis(AxisRotation::Axis::Y)) {
			transform.rotateYClamped(dt * rotation.getSpeedY(), -XM_PI, XM_PI);
		}
		if (rotation.hasAxis(AxisRotation::Axis::Z)) {
			transform.rotateZClamped(dt * rotation.getSpeedZ(), -XM_PI, XM_PI);
		}
	});
}
