#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"
#include "rendering\buffer\constant_buffer.h"
#include "geometry\frustum\frustum.h"
#include "resource\resource_mgr.h"
#include "scene\camera\skybox\skybox.h"


class Camera {
	public:
		Camera(ID3D11Device* device,
			   ID3D11DeviceContext* device_context,
			   u32 viewport_width,
			   u32 viewport_height);

		~Camera() = default;


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
		// Setters
		//----------------------------------------------------------------------------------

		void SetPosition(const float3& position);

		void SetRotation(const float3& rotation);

		// Set a new viewport
		void SetViewport(ID3D11DeviceContext* device_context, const D3D11_VIEWPORT& viewport) {
			this->viewport = viewport;
			BindViewport(device_context);
			UpdateProjectionMatrix();
		}

		// Change the viewport size
		void ResizeViewport(ID3D11DeviceContext* device_context, u32 width, u32 height) {
			viewport.Width  = static_cast<float>(width);
			viewport.Height = static_cast<float>(height);
			BindViewport(device_context);
			UpdateProjectionMatrix();
		}

		void SetViewportTopLeft(u32 top_left_x, u32 top_left_y) {
			viewport.TopLeftX = static_cast<float>(top_left_x);
			viewport.TopLeftY = static_cast<float>(top_left_y);
			UpdateProjectionMatrix();
		}

		// Set the depth range
		void SetZDepth(float z_near, float z_far) {
			this->z_near = z_near;
			this->z_far  = z_far;
		}

		// Change the FOV
		void SetFOV(float radians) {
			fov = radians;
			UpdateProjectionMatrix();
		}

		// Set the skybox for this camera
		void SetSkybox(const SkyBox& skybox) {
			this->skybox = skybox;
		}


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		// Get matrices
		const XMMATRIX GetViewMatrix()  const {
			return view_matrix;
		}
		const XMMATRIX GetProjMatrix()  const {
			return projection_matrix;
		}

		// Get the Frustum
		const Frustum& GetFrustum() const {
			return frustum;
		}

		// Get the Skybox
		const SkyBox& GetSkybox() const {
			return skybox;
		}

		// Get position, rotation, velocity (in units/ms)
		const float3 GetRotation() const {
			return float3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll));
		}

		const float3 GetPosition() const {
			float3 pos;
			XMStoreFloat3(&pos, position);
			return pos;
		}


	protected:
		// Bind the viewport to the pipeline
		void BindViewport(ID3D11DeviceContext* device_context) const {
			Pipeline::RS::BindViewports(device_context, 1, &viewport);
		}

		// Update the viewport after changing depth/width/height/etc...
		void UpdateProjectionMatrix();

		// Update the view matrix. Used after moving/rotating the camera.
		void UpdateViewMatrix();

		// Update the frustum. Used after the projection matrix or view matrix changes.
		void UpdateFrustum() {
			frustum.UpdateFrustum(view_matrix * projection_matrix);
		}


	protected:
		// Camera's constant buffer
		ConstantBuffer<CameraBuffer> buffer;

		// Camera's skybox
		SkyBox skybox;

		// Camera frustum
		Frustum frustum;

		// Viewport, FOV, depth, and aspect ratio
		D3D11_VIEWPORT viewport;
		float fov;
		float z_near;
		float z_far;
		float aspect_ratio;

		// Matrices
		XMMATRIX view_matrix;
		XMMATRIX projection_matrix;

		// Default vectors
		static constexpr XMVECTOR default_forward = { 0.0f, 0.0f, 1.0f, 0.0f };
		static constexpr XMVECTOR default_right   = { 1.0f, 0.0f, 0.0f, 0.0f };
		static constexpr XMVECTOR default_up      = { 0.0f, 1.0f, 0.0f, 0.0f };

		// Orientation vectors
		XMVECTOR camera_forward;
		XMVECTOR camera_right;
		XMVECTOR camera_up;

		// Camera position
		XMVECTOR position;

		// Pitch, yaw, roll
		float pitch;
		float yaw;
		float roll;
};