#pragma once

#include "components/camera/camera_base.h"


class PerspectiveCamera final : public CameraBase<PerspectiveCamera> {
public:
	PerspectiveCamera(ID3D11Device& device,
	                  u32 viewport_width,
	                  u32 viewport_height);

	~PerspectiveCamera() = default;

	// Change the FOV
	void setFOV(f32 radians) {
		fov = radians;
		updateProjectionMatrix();
	}


protected:
	// Update the projection matrix after changing depth/width/height/etc...
	void updateProjectionMatrix() override;


protected:
	// Camera FOV
	f32 fov;
};
