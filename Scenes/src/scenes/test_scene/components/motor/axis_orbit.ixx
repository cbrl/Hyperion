module;

#include <DirectXMath.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

export module components.motor.axis_orbit;

import ecs;
import math.directxmath;

using namespace DirectX;

export class AxisOrbit final : public ecs::Component {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	AxisOrbit() = default;
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
	void setAxis(f32_3 orbit_axis) {
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
	XMVECTOR axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	f32 speed = 1.0f;
};
