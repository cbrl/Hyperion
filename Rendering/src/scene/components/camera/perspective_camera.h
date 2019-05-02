#pragma once

#include "scene/components/camera/camera_base.h"


namespace render {

class PerspectiveCamera final : public CameraBase<PerspectiveCamera> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	PerspectiveCamera(ID3D11Device& device, u32_2 viewport_size);

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
	void setFOV(f32 radians) noexcept;

	[[nodiscard]]
	f32 getFOV() const noexcept;

	// Update the projection matrix after changing depth/width/height/etc...
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const override;


protected:
	// Camera FOV
	f32 fov;
};

} //namespace render