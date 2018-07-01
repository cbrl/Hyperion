#pragma once

#include "component/component.h"


class MouseRotation final : public Component<MouseRotation> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	MouseRotation() noexcept
		: sensitivity(0.1f)
		, max_rotation(XMConvertToRadians(89.0f), XM_PI) {
	}

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
	vec2_f32 getMaxRotation() const noexcept {
		return max_rotation;
	}

	void setMaxRotation(vec2_f32 angle) noexcept {
		max_rotation.x = (angle.x > abs_max.x) ? abs_max.x : angle.x;
		max_rotation.y = (angle.y > abs_max.y) ? abs_max.y : angle.y;
	}

	void setMaxRotationX(f32 angle) noexcept {
		max_rotation.x = angle > abs_max.x ? abs_max.x : angle;
	}

	void setMaxRotationY(f32 angle) noexcept {
		max_rotation.y = angle > abs_max.y ? abs_max.y : angle;
	}


private:
	f32      sensitivity;
	vec2_f32 max_rotation;

	static inline vec2_f32 abs_max{ XMConvertToRadians(89.0f), XM_PI };
};