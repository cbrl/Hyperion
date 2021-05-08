module;

#include "ecs.h"
#include "system/system.h"
#include "scene/components/transform/transform.h"

export module systems.motor.axis_orbit;

import components.motor.axis_orbit;


export class AxisOrbitSystem final : public ecs::System<AxisOrbitSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AxisOrbitSystem(ecs::ECS& ecs) : System(ecs) {
	}

	AxisOrbitSystem(const AxisOrbitSystem& system) = delete;
	AxisOrbitSystem(AxisOrbitSystem&& system) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~AxisOrbitSystem() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	AxisOrbitSystem& operator=(const AxisOrbitSystem& system) = delete;
	AxisOrbitSystem& operator=(AxisOrbitSystem&& system) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override {
		auto& ecs = this->getECS();

		ecs.forEach<Transform, AxisOrbit>([&](handle64 entity) {
			auto& transform = ecs.get<Transform>(entity);
			const auto& orbit = ecs.get<AxisOrbit>(entity);

			if (!orbit.isActive() || !transform.isActive())
				return;

			const auto units = static_cast<f32>(dtSinceLastUpdate().count() * orbit.getSpeed());
			transform.rotateAround(orbit.getAxis(), units);
		});
	}
};
