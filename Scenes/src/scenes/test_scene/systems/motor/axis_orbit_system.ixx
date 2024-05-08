module;

#include "datatypes/scalar_types.h"
#include "memory/handle/handle.h"

export module systems.motor.axis_orbit;

import ecs;
import components.motor.axis_orbit;
import rendering;


export class AxisOrbitSystem final : public ecs::System {
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
