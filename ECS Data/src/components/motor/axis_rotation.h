#pragma once

#include "component/component.h"


class AxisRotation final : public Component<AxisRotation> {
public:
	enum Axis : u8 {
		X = 1 << 0,
		Y = 1 << 1,
		Z = 1 << 2
	};


public:
	AxisRotation()
		: rotation_axis(0)
		, rotation_speeds(1.0f, 1.0f, 1.0f) {
	}

	~AxisRotation() = default;


	//----------------------------------------------------------------------------------
	// Axis
	//----------------------------------------------------------------------------------

	// Set the axis to rotate on
	void setAxis(Axis axis) {
		rotation_axis = axis;
	}

	// Add a rotation axis
	void addAxis(Axis axis) {
		rotation_axis |= axis;
	}

	// Get the rotation axes
	[[nodiscard]]
	u8 axis() const {
		return rotation_axis;
	}

	// Check if a rotation axis is enabled
	[[nodiscard]]
	bool hasAxis(Axis axis) const {
		return rotation_axis & axis;
	}


	//----------------------------------------------------------------------------------
	// Speed
	//----------------------------------------------------------------------------------

	void setSpeeds(float3 speeds) {
		rotation_speeds = std::move(speeds);
	}

	void setSpeedX(float speed) {
		rotation_speeds.x = speed;
	}

	void setSpeedY(float speed) {
		rotation_speeds.y = speed;
	}

	void setSpeedZ(float speed) {
		rotation_speeds.z = speed;
	}

	[[nodiscard]]
	float3 speeds() const {
		return rotation_speeds;
	}

	float speedX() const {
		return rotation_speeds.x;
	}

	float speedY() const {
		return rotation_speeds.y;
	}

	float speedZ() const {
		return rotation_speeds.z;
	}


private:
	u8     rotation_axis;
	float3 rotation_speeds;
};