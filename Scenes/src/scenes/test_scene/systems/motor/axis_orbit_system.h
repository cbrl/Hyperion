#pragma once

#include "system/system.h"


class AxisOrbitSystem final : public ecs::System<AxisOrbitSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AxisOrbitSystem(ecs::ECS& ecs);
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
	void update() override;
};