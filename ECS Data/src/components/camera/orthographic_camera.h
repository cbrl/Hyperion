#pragma once

#include "components/camera/camera_base.h"


class OrthographicCamera final : public CameraBase<OrthographicCamera> {
public:
	OrthographicCamera(ID3D11Device& device,
	                   u32 viewport_width,
	                   u32 viewport_height);

	~OrthographicCamera() = default;


	//----------------------------------------------------------------------------------
	// Orthographic Size
	//----------------------------------------------------------------------------------

	// Get the size of the camera's viewing volume {width, height}
	vec2_f32 size() const {
		return ortho_size;
	}

	// Set the size of the camera's viewing volume {width, height}
	void setSize(vec2_f32 size) {
		ortho_size = std::move(size);
		updateProjectionMatrix();
	}


private:
	// Update the projection matrix after changing depth/width/height/etc...
	void updateProjectionMatrix() override;


private:
	// The width and height of the camera's viewing volume
	vec2_f32 ortho_size;
};
