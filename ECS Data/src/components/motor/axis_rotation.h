#pragma once

#include "component/component.h"


class AxisRotation final : public Component<AxisRotation> {
public:
	//----------------------------------------------------------------------------------
	// Axis Enum
	//----------------------------------------------------------------------------------
	enum Axis : u8 {
		X   = 1 << 0,
		Y   = 1 << 1,
		Z   = 1 << 2,
		XY  = X | Y,
		YZ  = Y | Z,
		XYZ = X | Y | Z,
	};


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	AxisRotation() noexcept
		: rotation_axis(0)
		, rotation_speeds(1.0f, 1.0f, 1.0f) {
	}

	AxisRotation(const AxisRotation& rotation) = delete;
	AxisRotation(AxisRotation&& rotation) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------

	~AxisRotation() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	AxisRotation& operator=(const AxisRotation& rotation) = delete;
	AxisRotation& operator=(AxisRotation&& rotation) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Axis
	//----------------------------------------------------------------------------------

	// Set the axis to rotate on
	void setAxis(Axis axis) {
		rotation_axis = axis;
	}

	// Add a rotation axis
	void addAxis(Axis axis) {
		rotation_axis |= axis;
	}

	// Remove a rotation axis
	void removeAxis(Axis axis) {
		rotation_axis &= ~axis;
	}

	// Get the rotation axes
	[[nodiscard]]
	u8 getAxis() const {
		return rotation_axis;
	}

	// Check if a rotation axis is enabled
	[[nodiscard]]
	bool hasAxis(Axis axis) const {
		return rotation_axis & axis;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Speed
	//----------------------------------------------------------------------------------

	void setSpeeds(vec3_f32 speeds) {
		rotation_speeds = speeds;
	}

	void setSpeedX(f32 speed) {
		rotation_speeds.x = speed;
	}

	void setSpeedY(f32 speed) {
		rotation_speeds.y = speed;
	}

	void setSpeedZ(f32 speed) {
		rotation_speeds.z = speed;
	}

	[[nodiscard]]
	vec3_f32 speeds() const {
		return rotation_speeds;
	}

	[[nodiscard]]
	f32 getSpeedX() const {
		return rotation_speeds.x;
	}

	[[nodiscard]]
	f32 getSpeedY() const {
		return rotation_speeds.y;
	}

	[[nodiscard]]
	f32 getSpeedZ() const {
		return rotation_speeds.z;
	}


private:
	u8       rotation_axis;
	vec3_f32 rotation_speeds;
};