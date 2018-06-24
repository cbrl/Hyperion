#include "axis_rotation_system.h"
#include "engine/engine.h"
#include "components/motor/axis_rotation.h"
#include "components/transform/transform.h"


void AxisRotationSystem::update(const Engine& engine) {

	auto& ecs_engine = engine.getECS();
	const f32 dt     = static_cast<f32>(engine.getTimer().deltaTime());

	ecs_engine.forEachActive<AxisRotation>([&](AxisRotation& rotation) {
		
		auto transform = ecs_engine.getComponent<Transform>(rotation.getOwner());
		if (!transform) return;

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
