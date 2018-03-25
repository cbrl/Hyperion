#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "geometry\frustum\frustum.h"
#include "rendering\buffer\constant_buffer.h"


class Camera {
	public:
		Camera(ID3D11Device* device);

		Camera(ID3D11Device* device,
			   u32 viewport_width,
			   u32 viewport_height,
			   float fov,
			   float z_near,
			   float z_far);

		~Camera() = default;


		// Calculate new view matrix and velocity
		void UpdateMovement(float delta_time);


		// Update the constant buffer
		void UpdateBuffer(ID3D11DeviceContext* device_context) {
			buffer.UpdateData(device_context, CameraBuffer(position));
		}

		// Bind the constant buffer
		void BindBuffer(ID3D11DeviceContext* device_context, u32 slot) {
			buffer.Bind<Pipeline>(device_context, slot);
		}


		// Resize the viewport
		void OnResize(u32 width, u32 height);


		// Move and rotate the camera
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
		const XMMATRIX GetViewMatrix()  const { return view_matrix; }
		const XMMATRIX GetProjMatrix()  const { return projection_matrix; }
		const XMMATRIX GetOrthoMatrix() const { return ortho_matrix; }
		
		// Get Frustum
		const Frustum& GetFrustum() const { return frustum; }


		// Get position, rotation, velocity (in units/ms)
		float3 GetRotation() const { return float3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll)); }
		float3 GetVelocity() const { return velocity; }
		float3 GetPosition() const {
			float3 pos;
			XMStoreFloat3(&pos, position);
			return pos;
		}


	private:
		// Camera's constant buffer
		ConstantBuffer<CameraBuffer> buffer;

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

		Frustum frustum;

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

