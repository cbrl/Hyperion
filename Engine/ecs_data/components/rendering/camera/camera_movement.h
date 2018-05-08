#pragma once

#include "ecs\component\component.h"
#include "input\input.h"


class CameraMovement : public Component<CameraMovement> {
	public:
		CameraMovement(weak_ptr<Input> input)
			: input(input)
			, velocity(0.0f, 0.0f, 0.0f)
			, acceleration(0.00002f)
			, deceleration(0.00005f)
			, max_velocity(0.01f)
			, is_moving_x(false)
			, is_moving_y(false)
			, is_moving_z(false)

			, turn_sensitivity(0.1f)
			, roll_sensitivity(0.0015f) {
		}

		~CameraMovement() = default;


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		const weak_ptr<Input>& GetInput() const { return input; }

		float3 GetVelocity()     const { return velocity; }
		float  GetAcceleration() const { return acceleration; }
		float  GetDeceleration() const { return deceleration; }
		float  GetMaxVelocity()  const { return max_velocity; }

		float GetTurnSensitivity() const { return turn_sensitivity; }
		float GetRollSensitivity() const { return roll_sensitivity; }

		bool IsMovingX() const { return is_moving_x; }
		bool IsMovingY() const { return is_moving_y; }
		bool IsMovingZ() const { return is_moving_z; }


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		void SetVelocity(const float3& vel)         { velocity = vel; }
		void XM_CALLCONV SetVelocity(FXMVECTOR vel) { XMStoreFloat3(&velocity, vel); }

		void SetAcceleration(float accel)   { acceleration = accel; }
		void SetDeceleration(float decel)   { deceleration = decel; }
		void SetMaxVelocity(float max_vel)  { max_velocity = max_vel; }

		void SetTurnSensitivity(float sens) { turn_sensitivity = sens; }
		void SetRollSensitivity(float sens) { roll_sensitivity = sens; }

		void SetMovingX(bool moving) { is_moving_x = moving; }
		void SetMovingY(bool moving) { is_moving_y = moving; }
		void SetMovingZ(bool moving) { is_moving_z = moving; }


	private:
		weak_ptr<Input> input;

		// Position, veloctiy, acceleration (units per ms)
		float3 velocity;
		float  acceleration;
		float  deceleration;
		float  max_velocity;

		// Turning related variables
		float turn_sensitivity;
		float roll_sensitivity;

		// Booleans
		bool is_moving_x;
		bool is_moving_y;
		bool is_moving_z;
};