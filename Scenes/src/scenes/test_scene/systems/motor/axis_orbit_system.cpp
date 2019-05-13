#include "axis_orbit_system.h"
#include "engine/engine.h"
#include "scenes/test_scene/components/motor/axis_orbit.h"


void AxisOrbitSystem::update() {

	getECS().forEach<Transform, AxisOrbit>([&] (ecs::Entity& entity) {
		auto& transform   = *entity.getComponent<Transform>();
		const auto& orbit = *entity.getComponent<AxisOrbit>();

		if (!orbit.isActive() || !transform.isActive())
			return;

		const auto units = static_cast<f32>(dtSinceLastUpdate().count() * orbit.getSpeed());
		transform.rotateAround(orbit.getAxis(), units);
	});
}