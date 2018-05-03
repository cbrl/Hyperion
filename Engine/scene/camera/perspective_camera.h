#pragma once

#include "scene\camera\camera_base.h"
#include "scene\camera\skybox\skybox.h"


class PerspectiveCamera : public CameraBase {
	public:
		PerspectiveCamera(ID3D11Device* device,
						  ID3D11DeviceContext* device_context,
						  u32 viewport_width,
						  u32 viewport_height);

		~PerspectiveCamera() = default;

		// Change the FOV
		void SetFOV(float radians) {
			fov = radians;
			UpdateProjectionMatrix();
		}


	protected:
		// Update the projection matrix after changing depth/width/height/etc...
		void UpdateProjectionMatrix() override;


	protected:
		// Camera FOV
		float fov;
};