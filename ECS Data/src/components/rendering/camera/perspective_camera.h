#pragma once

#include "components/rendering/camera/camera_base.h"
#include "components/rendering/skybox/skybox.h"


class PerspectiveCamera final : public CameraBase<PerspectiveCamera> {
public:
	PerspectiveCamera(ID3D11Device& device,
	                  ID3D11DeviceContext& device_context,
	                  u32 viewport_width,
	                  u32 viewport_height);

	~PerspectiveCamera() = default;

	// Change the FOV
	void setFOV(float radians) {
		fov = radians;
		updateProjectionMatrix();
	}


protected:
	// Update the projection matrix after changing depth/width/height/etc...
	void updateProjectionMatrix() override;


protected:
	// Camera FOV
	float fov;
};
