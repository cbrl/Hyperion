#include "axis_orbit_system.h"
#include "engine/engine.h"


void AxisOrbitSystem::update(Engine& engine) {
	
	auto& scene      = engine.getScene();
	f64   delta_time = engine.getTimer().deltaTime<std::ratio<1,1>>();

	scene.forEach<AxisOrbit>([&] (AxisOrbit& orbit) {
	
		if (!orbit.isActive()) return;

		if (auto* transform = orbit.getOwner()->getComponent<Transform>()) {
			if (!transform->isActive()) return;

			const auto units = static_cast<f32>(delta_time * orbit.getSpeed());
			transform->rotateAround(orbit.getAxis(), units);
		}
	});
}