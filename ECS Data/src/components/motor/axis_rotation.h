#pragma once

#include "component/component.h"


enum class Axis {
	X,
	Y,
	Z,
	XY,
	XZ,
	YZ,
	XYZ
};


class AxisRotation final : public Component<AxisRotation> {
public:
	AxisRotation()
		: rotation_axis(Axis::Y)
		, rotation_speed(1.0f) {
	}

	~AxisRotation() = default;


	//----------------------------------------------------------------------------------
	// Axis
	//----------------------------------------------------------------------------------
	void setAxis(Axis axis) {
		rotation_axis = axis;
	}

	[[nodiscard]]
	Axis axis() const {
		return rotation_axis;
	}


	//----------------------------------------------------------------------------------
	// Speed
	//----------------------------------------------------------------------------------
	void setSpeed(float speed) {
		rotation_speed = speed;
	}

	[[nodiscard]]
	float speed() const {
		return rotation_speed;
	}


private:
	Axis  rotation_axis;
	float rotation_speed;
};