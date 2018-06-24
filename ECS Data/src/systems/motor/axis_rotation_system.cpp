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
			transform->rotateX(dt * rotation.getSpeedX());
		}
		if (rotation.hasAxis(AxisRotation::Axis::Y)) {
			transform->rotateY(dt * rotation.getSpeedY());
		}
		if (rotation.hasAxis(AxisRotation::Axis::Z)) {
			transform->rotateZ(dt * rotation.getSpeedZ());
		}
	});
}
