#include "axis_orbit_system.h"
#include "engine/engine.h"


void AxisOrbitSystem::update(const Engine& engine) {
	
	auto& ecs_engine = engine.getECS();
	f32   delta_time = static_cast<f32>(engine.getTimer().deltaTime());

	ecs_engine.forEach<AxisOrbit>([&] (AxisOrbit& orbit) {
	
		if (!orbit.isActive()) return;

		auto* transform = ecs_engine.getComponent<Transform>(orbit.getOwner());
		if (!transform) return;

		transform->rotateAround(orbit.getAxis(), delta_time * orbit.getSpeed());
	});
}