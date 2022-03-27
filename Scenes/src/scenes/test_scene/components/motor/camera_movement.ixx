module;

#include "directxmath/directxmath.h"

export module components.motor.camera_movement;

import ecs;

export class CameraMovement final : public ecs::Component {
	friend class CameraMotorSystem;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraMovement() = default;
	CameraMovement(const CameraMovement& movement) = delete;
	CameraMovement(CameraMovement&& movement) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~CameraMovement() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	CameraMovement& operator=(const CameraMovement& movement) = delete;
	CameraMovement& operator=(CameraMovement&& movement) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Velocity
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	f32_3 getVelocity() const {
		return velocity;
	}

	void setVelocity(const f32_3& vel) {
		velocity = vel;
	}

	void XM_CALLCONV setVelocity(FXMVECTOR vel) {
		XMStore(&velocity, vel);
	}

	[[nodiscard]]
	f32 getMaxVelocity() const {
		return max_velocity;
	}

	void setMaxVelocity(f32 max_vel) {
		max_velocity = max_vel;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Acceleration
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	f32 getAcceleration() const {
		return acceleration;
	}

	void setAcceleration(f32 accel) {
		acceleration = accel;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Deceleration
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	f32 getDeceleration() const {
		return deceleration;
	}

	void setDeceleration(f32 decel) {
		deceleration = decel;
	}

private:

	//----------------------------------------------------------------------------------
	// Member Functions - In Motion Flags
	//----------------------------------------------------------------------------------
	[[nodiscard]]
	bool isMovingX() const {
		return is_moving_x;
	}

	void setMovingX(bool moving) {
		is_moving_x = moving;
	}


	[[nodiscard]]
	bool isMovingY() const {
		return is_moving_y;
	}

	void setMovingY(bool moving) {
		is_moving_y = moving;
	}


	[[nodiscard]]
	bool isMovingZ() const {
		return is_moving_z;
	}

	void setMovingZ(bool moving) {
		is_moving_z = moving;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Position, veloctiy, acceleration (units per second)
	f32_3 velocity     = { 0.0f };
	f32   acceleration = 20.0f;
	f32   deceleration = 50.0f;
	f32   max_velocity = 10.0f;

	// Movement booleans
	bool is_moving_x = false;
	bool is_moving_y = false;
	bool is_moving_z = false;
};
