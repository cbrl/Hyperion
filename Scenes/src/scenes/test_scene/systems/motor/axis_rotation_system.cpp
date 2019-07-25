#include "axis_rotation_system.h"

#include "ecs.h"
#include "scene/components/transform/transform.h"
#include "scenes/test_scene/components/motor/axis_rotation.h"


AxisRotationSystem::AxisRotationSystem(ecs::ECS& ecs) : System(ecs) {

}

void AxisRotationSystem::update() {
	auto& ecs = this->getECS();

	ecs.forEach<Transform, AxisRotation>([&](handle64 entity) {
		auto& transform      = ecs.get<Transform>(entity);
		const auto& rotation = ecs.get<AxisRotation>(entity);

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
