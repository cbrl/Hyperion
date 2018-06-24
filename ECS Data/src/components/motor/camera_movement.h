#pragma once

#include "component/component.h"
#include "directx/directx_math.h"


class CameraMovement final : public Component<CameraMovement> {
public:
	CameraMovement()
		: velocity(0.0f, 0.0f, 0.0f)
		, acceleration(20.0f)
		, deceleration(50.0f)
		, max_velocity(10.0f)
		, roll_sensitivity(1.5f)
		, is_moving_x(false)

		, is_moving_y(false)
		, is_moving_z(false) {
	}

	~CameraMovement() = default;


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	f32_3 getVelocity() const {
		return velocity;
	}

	[[nodiscard]]
	f32 getAcceleration() const {
		return acceleration;
	}

	[[nodiscard]]
	f32 getDeceleration() const {
		return deceleration;
	}

	[[nodiscard]]
	f32 getMaxVelocity() const {
		return max_velocity;
	}

	[[nodiscard]]
	f32 getRollSensitivity() const {
		return roll_sensitivity;
	}

	[[nodiscard]]
	bool isMovingX() const {
		return is_moving_x;
	}

	[[nodiscard]]
	bool isMovingY() const {
		return is_moving_y;
	}

	[[nodiscard]]
	bool isMovingZ() const {
		return is_moving_z;
	}


	//----------------------------------------------------------------------------------
	// Setters
	//----------------------------------------------------------------------------------

	void setVelocity(const f32_3& vel) { velocity = vel; }
	void XM_CALLCONV setVelocity(FXMVECTOR vel) { XMStore(&velocity, vel); }

	void setAcceleration(f32 accel) { acceleration = accel; }
	void setDeceleration(f32 decel) { deceleration = decel; }
	void setMaxVelocity(f32 max_vel) { max_velocity = max_vel; }

	void setRollSensitivity(f32 sens) { roll_sensitivity = sens; }

	void setMovingX(bool moving) { is_moving_x = moving; }
	void setMovingY(bool moving) { is_moving_y = moving; }
	void setMovingZ(bool moving) { is_moving_z = moving; }


private:
	// Position, veloctiy, acceleration (units per second)
	f32_3 velocity;
	f32 acceleration;
	f32 deceleration;
	f32 max_velocity;

	// Roll speed multiplier
	f32 roll_sensitivity;

	// Movement booleans
	bool is_moving_x;
	bool is_moving_y;
	bool is_moving_z;
};
