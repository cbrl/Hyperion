#pragma once

#include <d3d11.h>
#include "util\math\math.h"
#include "util\datatypes\datatypes.h"
#include "rendering\buffer\buffers.h"

using namespace DirectX;

class Camera {
	public:
		Camera();
		~Camera() = default;

		Camera(u32 viewport_width, u32 viewport_height,
			   float fov, float z_near, float z_far);


		// Calculate new view matrix and velocity
		void Update(float delta_time);


		// Resize the viewport
		void OnResize(u32 width, u32 height) {
			viewport_width    = width;
			viewport_height   = height;
			aspect_ratio      = (float)viewport_width / (float)viewport_height;
			projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
			ortho_matrix      = XMMatrixOrthographicLH((float)viewport_width, (float)viewport_height, z_near, z_far);
		}


		// Move and rotate camera
		void Move(float3 units);
		void Rotate(float3 units);


		// Set position and rotation
		void SetPosition(float3 new_position) { 
			position = XMVectorSet(new_position.x, new_position.y, new_position.z, 0.0f);
		}
		void SetRotation(float3 rotation) {
			pitch = rotation.x;
			yaw   = rotation.y;
			roll  = rotation.z;
		}


		// Set depth and fov
		void SetDepthRange(float zNear, float zFar) {
			z_near = zNear;
			z_far  = zFar;
			ortho_matrix = XMMatrixOrthographicLH((float)viewport_width, (float)viewport_height, z_near, z_far);
		}
		void SetFOV(float radians) {
			fov = radians;
			projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
		}


		// Setters for movement speed related variables
		void SetAcceleration(float accel) { move_accel = accel; }
		void SetDeceleration(float decel) { move_decel = decel; }
		void SetMaxVelocity(float velocity) { max_velocity = velocity; }
		void SetSensitivity(float sensitivity) { turn_factor = sensitivity; }


		// Get matrices
		XMMATRIX GetViewMatrix()  const { return view_matrix; }
		XMMATRIX GetProjMatrix()  const { return projection_matrix; }
		XMMATRIX GetOrthoMatrix() const { return ortho_matrix; }


		// Get position, rotation, velocity (in units/ms)
		float3 GetRotation() const { return float3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll)); }
		float3 GetVelocity() const { return velocity; }
		float3 GetPosition() const {
			float3 pos;
			XMStoreFloat3(&pos, position);
			return pos;
		}


	private:
		// Viewport and depth range
		u32   viewport_width;
		u32   viewport_height;
		float z_near;
		float z_far;
		float fov;
		float aspect_ratio;

		XMMATRIX view_matrix;
		XMMATRIX projection_matrix;
		XMMATRIX ortho_matrix;

		// Default vectors
		static constexpr XMVECTOR default_forward = { 0.0f, 0.0f, 1.0f, 0.0f };
		static constexpr XMVECTOR default_right = { 1.0f, 0.0f, 0.0f, 0.0f };
		static constexpr XMVECTOR default_up = { 0.0f, 1.0f, 0.0f, 0.0f };

		// Looking vectors
		XMVECTOR camera_forward;
		XMVECTOR fps_forward;
		XMVECTOR camera_right;
		XMVECTOR camera_up;
		XMVECTOR look_at;
		XMVECTOR position;

		// Position, veloctiy, acceleration (units per ms)
		float3 move_units;
		float3 velocity;
		float  move_accel;
		float  move_decel;
		float  max_velocity;

		// Pitch, yaw, roll
		float pitch;
		float yaw;
		float roll;
		float turn_factor;
		const float max_pitch;

		// Booleans
		bool enable_free_look;
		bool fps_mode;
		bool is_moving_x;
		bool is_moving_y;
		bool is_moving_z;
};

