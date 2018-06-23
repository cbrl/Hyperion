#include "axis_rotation_system.h"
#include "engine/engine.h"
#include "components/motor/axis_rotation.h"
#include "components/transform/transform.h"


void AxisRotationSystem::update(const Engine& engine) {

	auto& ecs_engine = engine.getECS();
	const float dt   = static_cast<float>(engine.getTimer().deltaTime());

	ecs_engine.forEachActive<AxisRotation>([&](AxisRotation& rotation) {
		
		auto transform = ecs_engine.getComponent<Transform>(rotation.getOwner());
		if (!transform) return;

		const auto axis = rotation.axis();

		if (axis == Axis::X  ||
			axis == Axis::XY ||
			axis == Axis::XZ ||
			axis == Axis::XYZ) {
			transform->rotateX(dt * rotation.speed());
		}
		if (axis == Axis::Y  ||
		    axis == Axis::XY ||
		    axis == Axis::YZ ||
		    axis == Axis::XYZ) {
			transform->rotateY(dt * rotation.speed());
		}
		if (axis == Axis::Z  ||
		    axis == Axis::XZ ||
		    axis == Axis::YZ ||
		    axis == Axis::XYZ) {
			transform->rotateZ(dt * rotation.speed());
		}
	});
}
