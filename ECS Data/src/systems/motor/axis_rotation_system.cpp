#include "axis_rotation_system.h"

#include "engine/engine.h"
#include "components/motor/axis_rotation.h"
#include "scene/components/transform/transform.h"


void AxisRotationSystem::update(Engine& engine) {

	auto& scene = engine.getScene();
	auto& timer = engine.getTimer();

	const auto dt = static_cast<f32>(timer.deltaTime<std::ratio<1,1>>());

	scene.forEach<AxisRotation>([&](AxisRotation& rotation) {

		if (!rotation.isActive()) return;
		
		auto* transform = rotation.getOwner()->getComponent<Transform>();
		if (!transform) return;
		if (!transform->isActive()) return;

		if (rotation.hasAxis(AxisRotation::Axis::X)) {
			transform->rotateXClamped(dt * rotation.getSpeedX(), -XM_PI, XM_PI);
		}
		if (rotation.hasAxis(AxisRotation::Axis::Y)) {
			transform->rotateYClamped(dt * rotation.getSpeedY(), -XM_PI, XM_PI);
		}
		if (rotation.hasAxis(AxisRotation::Axis::Z)) {
			transform->rotateZClamped(dt * rotation.getSpeedZ(), -XM_PI, XM_PI);
		}
	});
}
