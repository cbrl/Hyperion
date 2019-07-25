#include "axis_orbit_system.h"
#include "ecs.h"
#include "scene/components/transform/transform.h"
#include "scenes/test_scene/components/motor/axis_orbit.h"


AxisOrbitSystem::AxisOrbitSystem(ecs::ECS& ecs) : System(ecs) {

}

void AxisOrbitSystem::update() {
	auto& ecs = this->getECS();

	ecs.forEach<Transform, AxisOrbit>([&](handle64 entity) {
		auto& transform   = ecs.get<Transform>(entity);
		const auto& orbit = ecs.get<AxisOrbit>(entity);

		if (!orbit.isActive() || !transform.isActive())
			return;

		const auto units = static_cast<f32>(dtSinceLastUpdate().count() * orbit.getSpeed());
		transform.rotateAround(orbit.getAxis(), units);
	});
}