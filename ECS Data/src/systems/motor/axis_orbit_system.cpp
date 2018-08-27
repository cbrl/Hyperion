#include "axis_orbit_system.h"
#include "engine/engine.h"


void AxisOrbitSystem::update(Engine& engine) {
	
	auto& ecs_engine = engine.getScene().getECS();
	f32   delta_time = static_cast<f32>(engine.getTimer().deltaTime());

	ecs_engine.forEach<AxisOrbit>([&] (AxisOrbit& orbit) {
	
		if (!orbit.isActive()) return;

		if (auto* transform = orbit.getOwner()->getComponent<Transform>()) {
			transform->rotateAround(orbit.getAxis(), delta_time * orbit.getSpeed());
		}
	});
}