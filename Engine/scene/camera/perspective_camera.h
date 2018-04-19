#pragma once

#include "scene\camera\camera_base.h"
#include "geometry\frustum\frustum.h"
#include "scene\camera\skybox\skybox.h"


class PerspectiveCamera : public CameraBase {
	public:
		PerspectiveCamera(ID3D11Device* device,
						  ID3D11DeviceContext* device_context,
						  u32 viewport_width,
						  u32 viewport_height);

		~PerspectiveCamera() = default;


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

		// Set the skybox for this camera
		void SetSkybox(const SkyBox& skybox) {
			this->skybox = skybox;
		}


		//----------------------------------------------------------------------------------
		// Getters
		//----------------------------------------------------------------------------------

		// Get the Frustum for this camera
		const Frustum& GetFrustum() const {
			return frustum;
		}

		// Get the camera's Skybox
		const SkyBox& GetSkybox() const {
			return skybox;
		}


	protected:
		// Update the view matrix. Used after moving/rotating the camera.
		void UpdateViewMatrix();

		// Update the projection matrix after changing depth/width/height/etc...
		void UpdateProjectionMatrix();

		// Update the frustum. Used after the projection matrix or view matrix changes.
		void UpdateFrustum() {
			frustum.UpdateFrustum(view_matrix * projection_matrix);
		}


	protected:
		// Camera's skybox
		SkyBox skybox;

		// Camera frustum
		Frustum frustum;
};