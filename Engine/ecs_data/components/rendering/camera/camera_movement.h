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
			, roll_sensitivity(0.00002f)
			, max_pitch(XMConvertToRadians(89.0f))
			, free_look(false) {
		}

		~CameraMovement() = default;


	public:
		weak_ptr<Input> input;

		// Position, veloctiy, acceleration (units per ms)
		float3 velocity;
		float  acceleration;
		float  deceleration;
		float  max_velocity;

		// Turning related variables
		float turn_sensitivity;
		float roll_sensitivity;
		const float max_pitch;

		// Booleans
		bool free_look;
		bool is_moving_x;
		bool is_moving_y;
		bool is_moving_z;
};