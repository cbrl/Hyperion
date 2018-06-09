#pragma once

#include "component/component.h"
#include "input/input.h"


class CameraMovement : public Component<CameraMovement> {
public:
	CameraMovement()
		: velocity(0.0f, 0.0f, 0.0f)
		, acceleration(0.00002f)
		, deceleration(0.00005f)
		, max_velocity(0.01f)
		, turn_sensitivity(0.1f)
		, roll_sensitivity(0.0015f)
		, is_moving_x(false)

		, is_moving_y(false)
		, is_moving_z(false) {
	}

	~CameraMovement() = default;


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------

	float3 getVelocity() const { return velocity; }
	float getAcceleration() const { return acceleration; }
	float getDeceleration() const { return deceleration; }
	float getMaxVelocity() const { return max_velocity; }

	float getTurnSensitivity() const { return turn_sensitivity; }
	float getRollSensitivity() const { return roll_sensitivity; }

	bool isMovingX() const { return is_moving_x; }
	bool isMovingY() const { return is_moving_y; }
	bool isMovingZ() const { return is_moving_z; }


	//----------------------------------------------------------------------------------
	// Setters
	//----------------------------------------------------------------------------------

	void setVelocity(const float3& vel) { velocity = vel; }
	void XM_CALLCONV setVelocity(FXMVECTOR vel) { XMStoreFloat3(&velocity, vel); }

	void setAcceleration(float accel) { acceleration = accel; }
	void setDeceleration(float decel) { deceleration = decel; }
	void setMaxVelocity(float max_vel) { max_velocity = max_vel; }

	void setTurnSensitivity(float sens) { turn_sensitivity = sens; }
	void setRollSensitivity(float sens) { roll_sensitivity = sens; }

	void setMovingX(bool moving) { is_moving_x = moving; }
	void setMovingY(bool moving) { is_moving_y = moving; }
	void setMovingZ(bool moving) { is_moving_z = moving; }


private:
	weak_ptr<Input> input;

	// Position, veloctiy, acceleration (units per ms)
	float3 velocity;
	float acceleration;
	float deceleration;
	float max_velocity;

	// Turning related variables
	float turn_sensitivity;
	float roll_sensitivity;

	// Booleans
	bool is_moving_x;
	bool is_moving_y;
	bool is_moving_z;
};
