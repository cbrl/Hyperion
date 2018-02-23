#pragma once

#include <d3d11.h>
#include "util\math\math.h"
#include "rendering\buffer\buffers.h"

using namespace DirectX;

class Camera {
	public:
		Camera();
		~Camera() = default;

		Camera(uint32_t viewport_width, uint32_t viewport_height,
			   float fov, float z_near, float z_far);


		void OnResize(uint32_t width, uint32_t height) {
			viewport_width = width;
			viewport_height = height;
			aspect_ratio = (float)viewport_width / (float)viewport_height;
			projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
			ortho_matrix = XMMatrixOrthographicLH((float)viewport_width, (float)viewport_height, z_near, z_far);
		}


		// Move and rotate camera
		void Move(XMFLOAT3 units);
		void Rotate(XMFLOAT3 units);


		// Calculate new view matrix and velocity
		void Update(float deltaTime);


		void SetPosition(XMFLOAT3 newPosition) { 
			position = XMVectorSet(newPosition.x, newPosition.y, newPosition.z, 0.0f);
		}

		void SetRotation(XMFLOAT3 rotation) {
			pitch = rotation.x;
			yaw   = rotation.y;
			roll  = rotation.z;
		}

		void SetDepthRange(float zNear, float zFar) {
			z_near = zNear;
			z_far = zFar;
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
		XMMATRIX GetWorldMatrix() const { return world_matrix; }
		XMMATRIX GetViewMatrix()  const { return view_matrix; }
		XMMATRIX GetProjMatrix()  const { return projection_matrix; }
		XMMATRIX GetOrthoMatrix() const { return ortho_matrix; }

		// Get position, rotation, velocity
		XMFLOAT3 GetRotation() const { return XMFLOAT3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll)); }
		XMFLOAT3 GetVelocity() const { return velocity; }
		XMFLOAT3 GetPosition() const {
			XMFLOAT3 pos;
			XMStoreFloat3(&pos, position);
			return pos;
		}


	private:
		uint32_t viewport_width;
		uint32_t viewport_height;
		float z_near;
		float z_far;
		float fov;
		float aspect_ratio;

		XMMATRIX world_matrix;
		XMMATRIX view_matrix;
		XMMATRIX projection_matrix;
		XMMATRIX ortho_matrix;

		// Default vectors
		static constexpr XMVECTOR default_forward = { 0.0f, 0.0f, 1.0f, 0.0f };
		static constexpr XMVECTOR default_right = { 1.0f, 0.0f, 0.0f, 0.0f };
		static constexpr XMVECTOR default_up = { 0.0f, 1.0f, 0.0f, 0.0f };

		// Looking vectors
		XMVECTOR    camera_forward;
		XMVECTOR    fps_forward;
		XMVECTOR    camera_right;
		XMVECTOR    camera_up;
		XMVECTOR    look_at;
		XMVECTOR    position;

		// Position, veloctiy, acceleration
		XMFLOAT3 move_units;
		XMFLOAT3 velocity;
		float    move_accel;
		float    move_decel;
		float    max_velocity;

		// Pitch, yaw, roll
		float pitch;
		float yaw;
		float roll;
		float turn_factor;
		const float max_pitch;

		// Booleans
		bool enable_free_look;
		bool fps_mode;
		bool is_moving;
};

