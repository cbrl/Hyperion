#pragma once

#include "directx/directx_math.h"
#include "component/component.h"


class AxisOrbit final : public Component<AxisOrbit> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	AxisOrbit()
		: axis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
		, speed(1.0f) {
	}

	AxisOrbit(const AxisOrbit& orbit) = delete;
	AxisOrbit(AxisOrbit&& orbit) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~AxisOrbit() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	AxisOrbit& operator=(const AxisOrbit& orbit) = delete;
	AxisOrbit& operator=(AxisOrbit&& orbit) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Axis
	//----------------------------------------------------------------------------------

	void setAxis(vec3_f32 orbit_axis) {
		axis = XMLoad(&orbit_axis);
	}

	void XM_CALLCONV setAxis(FXMVECTOR orbit_axis) {
		axis = orbit_axis;
	}

	[[nodiscard]]
	XMVECTOR XM_CALLCONV getAxis() const {
		return axis;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Speed
	//----------------------------------------------------------------------------------

	void setSpeed(f32 value) {
		speed = value;
	}

	[[nodiscard]]
	f32 getSpeed() const {
		return speed;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	XMVECTOR axis;
	f32 speed;
};