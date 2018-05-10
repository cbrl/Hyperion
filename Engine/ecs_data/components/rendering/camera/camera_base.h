#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "ecs\component\component.h"
#include "rendering\pipeline.h"
#include "rendering\buffer\buffers.h"
#include "rendering\buffer\constant_buffer.h"
#include "resource\resource_mgr.h"
#include "geometry\frustum\frustum.h"


template<typename T>
class CameraBase : public Component<T> {
	friend class CameraSystem;

	protected:
		CameraBase(ID3D11Device* device,
				   ID3D11DeviceContext* device_context,
				   u32 viewport_width,
				   u32 viewport_height)
			: buffer(device)
			, viewport({ 0.0f, 0.0f, 0.0f, 0.f, 0.0f, 1.0f })
			, view_matrix(XMMatrixIdentity())
			, projection_matrix(XMMatrixIdentity())
			, frustum(XMMatrixIdentity())
			, z_near(0.1f)
			, z_far(1000.0f) {
		}

		~CameraBase() = default;


	public:
		//----------------------------------------------------------------------------------
		// Constant Buffer
		//----------------------------------------------------------------------------------

		// Update the camera's constant buffer
		void XM_CALLCONV UpdateBuffer(ID3D11DeviceContext* device_context, FXMMATRIX world, FXMVECTOR position) {
			buffer.UpdateData(device_context, CameraBuffer(position, XMMatrixTranspose(world * view_matrix * projection_matrix)));
		}

		// Bind the camera's constant buffer
		void BindBuffer(ID3D11DeviceContext* device_context, u32 slot) {
			buffer.Bind<Pipeline>(device_context, slot);
		}


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		// Set a new viewport
		void SetViewport(ID3D11DeviceContext* device_context, const D3D11_VIEWPORT& viewport) {
			this->viewport = viewport;
			BindViewport(device_context);
			UpdateProjectionMatrix();
		}

		// Change the viewport size
		void ResizeViewport(ID3D11DeviceContext* device_context, u32 width, u32 height) {
			viewport.Width = static_cast<float>(width);
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
			this->z_far = z_far;
			UpdateProjectionMatrix();
		}


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

		// Get the Frustum for this camera
		const Frustum& GetFrustum() const {
			return frustum;
		}


	protected:
		// Bind the viewport to the pipeline
		void BindViewport(ID3D11DeviceContext* device_context) const {
			Pipeline::RS::BindViewports(device_context, 1, &viewport);
		}

		// Update the projection matrix after changing depth/width/height/etc...
		virtual void UpdateProjectionMatrix() = 0;

		// Update the view matrix. Used after moving/rotating the camera.
		void XM_CALLCONV UpdateViewMatrix(FXMVECTOR position, FXMVECTOR forward, FXMVECTOR up) {
			// Create a target vector
			XMVECTOR look_at = position + forward;

			// Create the new view matrix
			view_matrix = XMMatrixLookAtLH(position, look_at, up);

			// The frustum needs to be updated when the view matrix changes
			UpdateFrustum();
		}

		// Update the frustum. Used after the projection matrix or view matrix changes.
		void UpdateFrustum() {
			frustum.UpdateFrustum(view_matrix * projection_matrix);
		}


	protected:
		// The camera's constant buffer
		ConstantBuffer<CameraBuffer> buffer;

		// Camera frustum
		Frustum frustum;

		// Viewport and z depth
		D3D11_VIEWPORT viewport;
		float z_near;
		float z_far;

		// Matrices
		XMMATRIX view_matrix;
		XMMATRIX projection_matrix;

		// Default vectors
		static constexpr XMVECTOR default_forward = { 0.0f, 0.0f, 1.0f, 0.0f };
		static constexpr XMVECTOR default_right   = { 1.0f, 0.0f, 0.0f, 0.0f };
		static constexpr XMVECTOR default_up      = { 0.0f, 1.0f, 0.0f, 0.0f };
};