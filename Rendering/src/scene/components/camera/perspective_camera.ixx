module;

#include "datatypes/types.h"

#include "directx/d3d11.h"
#include "directxmath/directxmath.h"

export module components.camera.perspective_camera;

import rendering.camera_base;


export class PerspectiveCamera final : public render::CameraBase<PerspectiveCamera> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	PerspectiveCamera(ID3D11Device& device, u32_2 viewport_size) : CameraBase(device) {
		viewport.setSize(viewport_size);
	}

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
	void setFOV(f32 radians) noexcept {
		fov = radians;
	}

	[[nodiscard]]
	f32 getFOV() const noexcept {
		return fov;
	}

	// Update the projection matrix after changing depth/width/height/etc...
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const override {
		const f32 aspect_ratio = viewport.getAspectRatio();
		return XMMatrixPerspectiveFovLH(fov, aspect_ratio, depth[0], depth[1]);
	}

protected:
	
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Camera FOV
	f32 fov = XM_PI / 4.0f;
};
