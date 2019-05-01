#pragma once

#include "system/system.h"

class Input;

class MouseRotationSystem final : public ecs::System<MouseRotationSystem> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MouseRotationSystem(const Input& input);
	MouseRotationSystem(const MouseRotationSystem&) = delete;
	MouseRotationSystem(MouseRotationSystem&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~MouseRotationSystem() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	MouseRotationSystem& operator=(const MouseRotationSystem&) = delete;
	MouseRotationSystem& operator=(MouseRotationSystem&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	const Input& input;
};