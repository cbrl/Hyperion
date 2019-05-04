#include "axis_orbit_system.h"
#include "engine/engine.h"
#include "scenes/test_scene/components/motor/axis_orbit.h"


void AxisOrbitSystem::update() {

	getECS().forEach<AxisOrbit>([&] (AxisOrbit& orbit) {
		if (!orbit.isActive()) return;

		if (auto* transform = orbit.getOwner()->getComponent<Transform>()) {
			if (!transform->isActive()) return;

			const auto units = static_cast<f32>(dtSinceLastUpdate().count() * orbit.getSpeed());
			transform->rotateAround(orbit.getAxis(), units);
		}
	});
}