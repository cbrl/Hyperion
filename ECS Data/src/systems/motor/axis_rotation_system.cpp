#include "axis_rotation_system.h"

#include "engine/engine.h"
#include "components/motor/axis_rotation.h"
#include "scene/components/transform/transform.h"


void AxisRotationSystem::update(Engine& engine) {

	auto& scene = engine.getScene();

	scene.forEach<AxisRotation>([&](AxisRotation& rotation) {

		if (!rotation.isActive()) return;
		
		auto* transform = rotation.getOwner()->getComponent<Transform>();
		if (!transform) return;
		if (!transform->isActive()) return;

		if (rotation.hasAxis(AxisRotation::Axis::X)) {
			transform->rotateXClamped(dtSinceLastUpdate() * rotation.getSpeedX(), -XM_PI, XM_PI);
		}
		if (rotation.hasAxis(AxisRotation::Axis::Y)) {
			transform->rotateYClamped(dtSinceLastUpdate() * rotation.getSpeedY(), -XM_PI, XM_PI);
		}
		if (rotation.hasAxis(AxisRotation::Axis::Z)) {
			transform->rotateZClamped(dtSinceLastUpdate() * rotation.getSpeedZ(), -XM_PI, XM_PI);
		}
	});
}
