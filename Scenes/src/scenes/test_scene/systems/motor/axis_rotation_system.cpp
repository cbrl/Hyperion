#include "axis_rotation_system.h"

#include "engine/engine.h"
#include "scene/components/transform/transform.h"
#include "scenes/test_scene/components/motor/axis_rotation.h"


void AxisRotationSystem::update() {

	getECS().forEach<AxisRotation>([&](AxisRotation& rotation) {

		if (!rotation.isActive()) return;
		
		auto* transform = rotation.getOwner()->getComponent<render::Transform>();
		if (!transform) return;
		if (!transform->isActive()) return;

		const auto dt = static_cast<f32>(dtSinceLastUpdate().count());

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
