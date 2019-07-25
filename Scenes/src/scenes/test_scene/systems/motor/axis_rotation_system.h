#pragma once

#include "system/system.h"


class AxisRotationSystem final : public ecs::System<AxisRotationSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AxisRotationSystem(ecs::ECS& ecs);
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
	void update() override;
};