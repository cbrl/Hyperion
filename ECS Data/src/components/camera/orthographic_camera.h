#pragma once

#include "components/camera/camera_base.h"


class OrthographicCamera final : public CameraBase<OrthographicCamera> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	OrthographicCamera(ID3D11Device& device, vec2_u32 viewport_size);

	OrthographicCamera(const OrthographicCamera& camera) = delete;
	OrthographicCamera(OrthographicCamera&& camera) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~OrthographicCamera() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	OrthographicCamera& operator=(const OrthographicCamera& camera) = delete;
	OrthographicCamera& operator=(OrthographicCamera&& camera) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Get the size of the camera's viewing volume {width, height}
	[[nodiscard]]
	vec2_f32 getSize() const {
		return ortho_size;
	}

	// Set the size of the camera's viewing volume {width, height}
	void setSize(vec2_f32 size) {
		ortho_size = size;
	}

	// Update the projection matrix after changing depth/width/height/etc...
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const override;


private:
	// The width and height of the camera's viewing volume
	vec2_f32 ortho_size;
};
