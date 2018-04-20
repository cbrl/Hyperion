#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "resource\resource_mgr.h"
#include "geometry\frustum\frustum.h"


class CameraBase {
	protected:
		CameraBase(ID3D11Device* device,
				   ID3D11DeviceContext* device_context,
				   u32 viewport_width,
				   u32 viewport_height);

		~CameraBase() = default;

	public:
		//----------------------------------------------------------------------------------
		// Constant Buffer
		//----------------------------------------------------------------------------------

		// Update the camera's constant buffer
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

		virtual void SetRotation(const float3& rotation) = 0;

		virtual void SetPosition(const float3& position) = 0;

		// Set the depth range
		virtual void SetZDepth(float z_near, float z_far) = 0;

		// Set a new viewport
		virtual void SetViewport(ID3D11DeviceContext* device_context, const D3D11_VIEWPORT& viewport) = 0;

		// Change the viewport size
		virtual void ResizeViewport(ID3D11DeviceContext* device_context, u32 width, u32 height) = 0;

		// Set the location of the top left corner of the viewport
		virtual void SetViewportTopLeft(u32 top_left_x, u32 top_left_y) = 0;


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		// Get the camera's view matrix
		const XMMATRIX GetViewMatrix() const {
			return view_matrix;
		}

		// Get the camera's projection matrix
		const XMMATRIX GetProjMatrix() const {
			return projection_matrix;
		}

		// Get the camera's current rotation
		const float3 GetRotation() const {
			return float3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll));
		}

		// Get the camera's current position
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

		// Update the projection matrix after changing depth/width/height/etc...
		virtual void UpdateProjectionMatrix() = 0;

		// Update the view matrix. Used after moving/rotating the camera.
		virtual void UpdateViewMatrix() = 0;

		// Update the frustum. Used after the projection matrix or view matrix changes.
		void UpdateFrustum() {
			frustum.UpdateFrustum(view_matrix * projection_matrix);
		}


	protected:
		// Camera's constant buffer
		ConstantBuffer<CameraBuffer> buffer;

		// Camera frustum
		Frustum frustum;

		// Viewport, FOV, depth
		D3D11_VIEWPORT viewport;
		float fov;
		float z_near;
		float z_far;

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