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
	float getAcceleration() const {
		return acceleration;
	}

	[[nodiscard]]
	float getDeceleration() const {
		return deceleration;
	}

	[[nodiscard]]
	float getMaxVelocity() const {
		return max_velocity;
	}

	[[nodiscard]]
	float getRollSensitivity() const {
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

	void setAcceleration(float accel) { acceleration = accel; }
	void setDeceleration(float decel) { deceleration = decel; }
	void setMaxVelocity(float max_vel) { max_velocity = max_vel; }

	void setRollSensitivity(float sens) { roll_sensitivity = sens; }

	void setMovingX(bool moving) { is_moving_x = moving; }
	void setMovingY(bool moving) { is_moving_y = moving; }
	void setMovingZ(bool moving) { is_moving_z = moving; }


private:
	// Position, veloctiy, acceleration (units per second)
	f32_3 velocity;
	float acceleration;
	float deceleration;
	float max_velocity;

	// Roll speed multiplier
	float roll_sensitivity;

	// Movement booleans
	bool is_moving_x;
	bool is_moving_y;
	bool is_moving_z;
};
