#include "axis_orbit_system.h"
#include "engine/engine.h"
#include "scenes/test_scene/components/motor/axis_orbit.h"


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