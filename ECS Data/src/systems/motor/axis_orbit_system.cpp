#include "axis_orbit_system.h"
#include "engine/engine.h"


void AxisOrbitSystem::update(Engine& engine) {
	
	auto& scene      = engine.getScene();
	f32   delta_time = static_cast<f32>(engine.getTimer().deltaTime());

	scene.forEach<AxisOrbit>([&] (AxisOrbit& orbit) {
	
		if (!orbit.isActive()) return;

		if (auto* transform = orbit.getOwner()->getComponent<Transform>()) {
			transform->rotateAround(orbit.getAxis(), delta_time * orbit.getSpeed());
		}
	});
}