#pragma once

#include "components/camera/camera_base.h"


class PerspectiveCamera final : public CameraBase<PerspectiveCamera> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	PerspectiveCamera(ID3D11Device& device,
	                  u32 viewport_width,
	                  u32 viewport_height);

	PerspectiveCamera(const PerspectiveCamera& camera) = delete;
	PerspectiveCamera(PerspectiveCamera&& camera) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~PerspectiveCamera() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	PerspectiveCamera& operator=(const PerspectiveCamera& camera) = delete;
	PerspectiveCamera& operator=(PerspectiveCamera&& caemra) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

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
