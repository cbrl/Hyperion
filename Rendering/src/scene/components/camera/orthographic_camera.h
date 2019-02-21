#pragma once

#include "scene/components/camera/camera_base.h"


class OrthographicCamera final : public CameraBase<OrthographicCamera> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	OrthographicCamera(ID3D11Device& device, u32_2 viewport_size);

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
	f32_2 getSize() const noexcept;

	// Set the size of the camera's viewing volume {width, height}
	void setSize(f32_2 size) noexcept;

	// Update the projection matrix after changing depth/width/height/etc...
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const override;


private:
	// The width and height of the camera's viewing volume
	f32_2 ortho_size;
};
