#pragma once

#include "scene\camera\camera_base.h"


class OrthographicCamera : public CameraBase {
	public:
		OrthographicCamera(ID3D11Device* device,
						   ID3D11DeviceContext* device_context,
						   u32 viewport_width,
						   u32 viewport_height);

		~OrthographicCamera() = default;


	protected:
		// Update the projection matrix after changing depth/width/height/etc...
		void UpdateProjectionMatrix();
};