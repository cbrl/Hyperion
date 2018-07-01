#pragma once

#include "component/component.h"
#include "directx/directx_math.h"


class CameraMovement final : public Component<CameraMovement> {
	friend class CameraMotorSystem;

public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	CameraMovement() noexcept
		: velocity(0.0f, 0.0f, 0.0f)
		, acceleration(20.0f)
		, deceleration(50.0f)
		, max_velocity(10.0f)
		, is_moving_x(false)
		, is_moving_y(false)
		, is_moving_z(false) {
	}

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
	vec3_f32 getVelocity() const {
		return velocity;
	}

	void setVelocity(const vec3_f32& vel) {
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
	// Member Functions - In Motion
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


private:
	// Position, veloctiy, acceleration (units per second)
	vec3_f32 velocity;
	f32 acceleration;
	f32 deceleration;
	f32 max_velocity;

	// Movement booleans
	bool is_moving_x;
	bool is_moving_y;
	bool is_moving_z;
};
