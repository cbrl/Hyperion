module;

#include "datatypes/scalar_types.h"
#include "memory/handle/handle.h"

export module systems.motor.axis_rotation;

import ecs;
import rendering;
import math.directxmath;


export class AxisRotationSystem final : public ecs::System {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AxisRotationSystem(ecs::ECS& ecs) : System(ecs) {
	}

	AxisRotationSystem(const AxisRotationSystem& system) = delete;
	AxisRotationSystem(AxisRotationSystem&& system) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~AxisRotationSystem() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	AxisRotationSystem& operator=(const AxisRotationSystem& system) = delete;
	AxisRotationSystem& operator=(AxisRotationSystem&& system) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override {
		auto& ecs = this->getECS();

		ecs.forEach<Transform, AxisRotation>([&](handle64 entity) {
			auto& transform = ecs.get<Transform>(entity);
			const auto& rotation = ecs.get<AxisRotation>(entity);

			if (!rotation.isActive() || !transform.isActive())
				return;

			const auto dt = static_cast<f32>(dtSinceLastUpdate().count());

			if (rotation.hasAxis(AxisRotation::Axis::X)) {
				transform.rotateXClamped(dt * rotation.getSpeedX(), -XM_PI, XM_PI);
			}
			if (rotation.hasAxis(AxisRotation::Axis::Y)) {
				transform.rotateYClamped(dt * rotation.getSpeedY(), -XM_PI, XM_PI);
			}
			if (rotation.hasAxis(AxisRotation::Axis::Z)) {
				transform.rotateZClamped(dt * rotation.getSpeedZ(), -XM_PI, XM_PI);
			}
		});
	}
};
