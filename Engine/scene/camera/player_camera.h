#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "scene\camera\camera.h"


class PlayerCamera final : public Camera {
	public:
		PlayerCamera(ID3D11Device* device,
					 ID3D11DeviceContext* device_context,
					 u32 viewport_width,
					 u32 viewport_height);

		~PlayerCamera() = default;


		//----------------------------------------------------------------------------------
		// Movement
		//----------------------------------------------------------------------------------

		void Move(float3 units);
		void Rotate(float3 units);

		// Calculate new view matrix, position, and velocity
		void UpdateMovement(float delta_time);


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		// Set movement speed related variables
		void SetAcceleration(float accel)      { acceleration = accel; }
		void SetDeceleration(float decel)      { deceleration = decel; }
		void SetMaxVelocity(float velocity)    { max_velocity = velocity; }
		void SetSensitivity(float sensitivity) { turn_factor = sensitivity; }

		// Set camera movement style
		void SetFreeLook(bool enabled) {
			if (!enabled && free_look) {
				camera_forward = default_forward;
				camera_right   = default_right;
				camera_up      = default_up;
			}
			free_look = enabled;
		}


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		// Camera movement style
		bool IsFreeLookEnabled() const { return free_look; }

		// Get movement speed related variables
		const float GetAcceleration() const { return acceleration; }
		const float GetDeleceration() const { return deceleration; }
		const float GetMaxVelocity()  const { return max_velocity; }
		const float GetSensitivity()  const { return turn_factor; }

		// Get velocity (in units/ms)
		const float3 GetVelocity() const { return velocity; }


	private:
		// Decelerate the camera if it isn't moving
		void Decelerate(float delta_time);


	private:
		// Position, veloctiy, acceleration (units per ms)
		float3 move_units;
		float3 velocity;
		float  acceleration;
		float  deceleration;
		float  max_velocity;

		// Turning related variables
		float turn_factor;
		const float max_pitch;

		// Booleans
		bool free_look;
		bool is_moving_x;
		bool is_moving_y;
		bool is_moving_z;
};

