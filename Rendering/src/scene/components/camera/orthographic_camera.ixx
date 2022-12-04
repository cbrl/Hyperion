module;

#include "datatypes/types.h"

#include "directx/d3d11.h"
#include "directxmath/directxmath.h"

export module components.camera.orthographic_camera;

import components.camera.camera_base;


export class OrthographicCamera final : public render::CameraBase<OrthographicCamera> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	OrthographicCamera(ID3D11Device& device, u32_2 viewport_size) : CameraBase(device) {
		viewport.setSize(viewport_size);
	}

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
	f32_2 getSize() const noexcept {
		return ortho_size;
	}

	// Set the size of the camera's viewing volume {width, height}
	void setSize(f32_2 size) noexcept {
		ortho_size = size;
	}

	// Update the projection matrix after changing depth/width/height/etc...
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const override {
		// Recalculate the projection matrix
		return XMMatrixOrthographicLH(ortho_size[0], ortho_size[1], depth[0], depth[1]);
	}


private:
	// The width and height of the camera's viewing volume
	f32_2 ortho_size = {1.0f, 1.0f};
};
