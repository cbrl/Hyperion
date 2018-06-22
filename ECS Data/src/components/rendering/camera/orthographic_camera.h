#pragma once

#include "components/rendering/camera/camera_base.h"


class OrthographicCamera final : public CameraBase<OrthographicCamera> {
public:
	OrthographicCamera(ID3D11Device& device,
	                   ID3D11DeviceContext& device_context,
	                   u32 viewport_width,
	                   u32 viewport_height);

	~OrthographicCamera() = default;


	//----------------------------------------------------------------------------------
	// Orthographic Size
	//----------------------------------------------------------------------------------

	// Get the size of the camera's viewing volume {width, height}
	float2 size() const {
		return ortho_size;
	}

	// Set the size of the camera's viewing volume {width, height}
	void setSize(float2 size) {
		ortho_size = std::move(size);
		updateProjectionMatrix();
	}


private:
	// Update the projection matrix after changing depth/width/height/etc...
	void updateProjectionMatrix() override;


private:
	// The width and height of the camera's viewing volume
	float2 ortho_size;
};
