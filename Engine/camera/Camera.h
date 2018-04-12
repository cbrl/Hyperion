#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "geometry\frustum\frustum.h"
#include "rendering\buffer\constant_buffer.h"
#include "resource\resource_mgr.h"
#include "resource\skybox\skybox.h"


class Camera final {
	private:
		Camera(ID3D11Device* device);

	public:
		Camera(ID3D11Device* device,
			   ResourceMgr& resource_mgr,
			   u32 viewport_width,
			   u32 viewport_height,
			   float fov,
			   float z_near,
			   float z_far,
			   wstring skybox_filename);

		~Camera() = default;


		// Resize the viewport
		void ResizeViewport(u32 width, u32 height);


		//----------------------------------------------------------------------------------
		// Constant Buffer
		//----------------------------------------------------------------------------------

		// Update the constant buffer
		void UpdateBuffer(ID3D11DeviceContext* device_context) {

			XMMATRIX world = XMMatrixTranslationFromVector(position);

			buffer.UpdateData(device_context, CameraBuffer(position, XMMatrixTranspose(world * view_matrix * projection_matrix)));
		}

		// Bind the camera's constant buffer
		void BindBuffer(ID3D11DeviceContext* device_context, u32 slot) {
			buffer.Bind<Pipeline>(device_context, slot);
		}


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

		void SetPosition(float3 position) { 
			this->position = XMVectorSet(position.x, position.y, position.z, 0.0f);
		}

		void SetRotation(float3 rotation) {
			pitch = rotation.x;
			yaw   = rotation.y;
			roll  = rotation.z;
		}

		void SetDepthRange(float zNear, float zFar) {
			z_near = zNear;
			z_far  = zFar;
			ortho_matrix = XMMatrixOrthographicLH((float)viewport_width, (float)viewport_height, z_near, z_far);
		}

		void SetFOV(float radians) {
			fov = radians;
			projection_matrix = XMMatrixPerspectiveFovLH(fov, aspect_ratio, z_near, z_far);
		}

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

		// Get matrices
		const XMMATRIX GetViewMatrix()  const { return view_matrix; }
		const XMMATRIX GetProjMatrix()  const { return projection_matrix; }
		const XMMATRIX GetOrthoMatrix() const { return ortho_matrix; }
		
		// Get the Frustum
		const Frustum& GetFrustum() const { return frustum; }

		// Get the Skybox
		const SkyBox& GetSkybox() const { return skybox; }

		// Camera movement style
		bool IsFreeLookEnabled() const { return free_look; }

		// Get movement speed related variables
		const float GetAcceleration() const { return acceleration; }
		const float GetDeleceration() const { return deceleration; }
		const float GetMaxVelocity()  const { return max_velocity; }
		const float GetSensitivity()  const { return turn_factor; }

		// Get position, rotation, velocity (in units/ms)
		const float3 GetRotation() const { return float3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll)); }
		const float3 GetVelocity() const { return velocity; }
		const float3 GetPosition() const {
			float3 pos;
			XMStoreFloat3(&pos, position);
			return pos;
		}


	private:
		// Camera's constant buffer
		ConstantBuffer<CameraBuffer> buffer;

		// Camera's skybox
		SkyBox skybox;

		// Camera frustum
		Frustum frustum;

		// Viewport size and depth range
		u32   viewport_width;
		u32   viewport_height;
		float z_near;
		float z_far;
		float fov;
		float aspect_ratio;

		// Matrices
		XMMATRIX view_matrix;
		XMMATRIX projection_matrix;
		XMMATRIX ortho_matrix;

		// Default vectors
		static constexpr XMVECTOR default_forward = { 0.0f, 0.0f, 1.0f, 0.0f };
		static constexpr XMVECTOR default_right = { 1.0f, 0.0f, 0.0f, 0.0f };
		static constexpr XMVECTOR default_up = { 0.0f, 1.0f, 0.0f, 0.0f };

		// Orientation vectors
		XMVECTOR camera_forward;
		XMVECTOR fps_forward;
		XMVECTOR camera_right;
		XMVECTOR camera_up;

		// Target vector
		XMVECTOR look_at;

		// Camera position
		XMVECTOR position;

		// Position, veloctiy, acceleration (units per ms)
		float3 move_units;
		float3 velocity;
		float  acceleration;
		float  deceleration;
		float  max_velocity;

		// Pitch, yaw, roll
		float pitch;
		float yaw;
		float roll;
		float turn_factor;
		const float max_pitch;

		// Booleans
		bool free_look;
		bool is_moving_x;
		bool is_moving_y;
		bool is_moving_z;
};

