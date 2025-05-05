module;

#include <DirectXMath.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

export module components.motor.mouse_rotation;

import ecs;
import math.directxmath;

using namespace DirectX;

export class MouseRotation final : public ecs::Component {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	MouseRotation() = default;
	MouseRotation(const MouseRotation& rotation) = delete;
	MouseRotation(MouseRotation&& rotation) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~MouseRotation() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	MouseRotation& operator=(const MouseRotation& rotation) = delete;
	MouseRotation& operator=(MouseRotation&& rotation) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Sensitivity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	f32 getSensitivity() const noexcept {
		return sensitivity;
	}

	void setSensitivity(f32 value) {
		sensitivity = value;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Max Rotation
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	f32_2 getMaxRotation() const noexcept {
		return max_rotation;
	}

	void setMaxRotation(f32_2 angle) noexcept {
		max_rotation[0] = (angle[0] > abs_max[0]) ? abs_max[0] : angle[0];
		max_rotation[1] = (angle[1] > abs_max[1]) ? abs_max[1] : angle[1];
	}

	void setMaxRotationX(f32 angle) noexcept {
		max_rotation[0] = angle > abs_max[0] ? abs_max[0] : angle;
	}

	void setMaxRotationY(f32 angle) noexcept {
		max_rotation[1] = angle > abs_max[1] ? abs_max[1] : angle;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	f32   sensitivity  = 0.1f;
	f32_2 max_rotation = abs_max;

	static constexpr f32_2 abs_max = { XMConvertToRadians(89.0f), XM_PI };
};
