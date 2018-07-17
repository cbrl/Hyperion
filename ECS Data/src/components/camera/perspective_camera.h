#pragma once

#include "components/camera/camera_base.h"


class PerspectiveCamera final : public CameraBase<PerspectiveCamera> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	PerspectiveCamera(ID3D11Device& device, vec2_u32 viewport_size);

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

	// Change the field of view
	void setFOV(f32 radians) {
		fov = radians;
	}

	[[nodiscard]]
	f32 getFOV() const {
		return fov;
	}

	// Update the projection matrix after changing depth/width/height/etc...
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const override;


protected:
	// Camera FOV
	f32 fov;
};
