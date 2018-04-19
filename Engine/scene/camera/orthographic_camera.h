#pragma once

#include "scene\camera\camera_base.h"


class OrthographicCamera : public CameraBase {
	public:
		OrthographicCamera(ID3D11Device* device,
						   ID3D11DeviceContext* device_context,
						   u32 viewport_width,
						   u32 viewport_height);
		~OrthographicCamera() = default;


		//----------------------------------------------------------------------------------
		// Setters
		//----------------------------------------------------------------------------------

		void SetRotation(const float3& rotation);

		void SetPosition(const float3& position) {
			this->position = XMLoadFloat3(&position);
			UpdateViewMatrix();
		}

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

		// Change the FOV
		void SetFOV(float radians) {
			fov = radians;
			UpdateProjectionMatrix();
		}


	protected:
		// Update the view matrix. Used after moving/rotating the camera.
		void UpdateViewMatrix();

		// Update the projection matrix after changing depth/width/height/etc...
		void UpdateProjectionMatrix();
};