#include "axis_orbit_system.h"
#include "engine/engine.h"


void AxisOrbitSystem::update(Engine& engine) {
	
	auto& scene      = engine.getScene();

	scene.forEach<AxisOrbit>([&] (AxisOrbit& orbit) {
	
		if (!orbit.isActive()) return;

		if (auto* transform = orbit.getOwner()->getComponent<Transform>()) {
			if (!transform->isActive()) return;

			const auto units = static_cast<f32>(dtSinceLastUpdate() * orbit.getSpeed());
			transform->rotateAround(orbit.getAxis(), units);
		}
	});
}