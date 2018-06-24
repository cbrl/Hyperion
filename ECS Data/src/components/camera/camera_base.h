#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "component/component.h"
#include "pipeline.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "display/viewport.h"
#include "components/camera/skybox/skybox.h"


template<typename T>
class CameraBase : public Component<T> {
	friend class CameraSystem;

protected:
	CameraBase(ID3D11Device& device)
		: buffer(device)
		, z_near(0.1f)
		, z_far(1000.0f)
		, sky(device)
		, projection_matrix(XMMatrixIdentity()) {

		viewport.setDepth(0.0f, 1.0f);
	}

	~CameraBase() = default;


public:
	// Bind the viewport to the pipeline
	void bindViewport(ID3D11DeviceContext& device_context) const {
		viewport.bind(device_context);
	}


	//----------------------------------------------------------------------------------
	// Constant Buffer
	//----------------------------------------------------------------------------------

	// Update the camera's constant buffer
	void XM_CALLCONV updateBuffer(ID3D11DeviceContext& device_context,
	                              FXMMATRIX camera_to_world,
	                              CXMMATRIX world_to_camera) const {

		buffer.updateData(device_context,
		                  CameraBuffer(XMMatrixTranspose(camera_to_world),
		                               XMMatrixTranspose(world_to_camera),
		                               XMMatrixTranspose(projection_matrix),
		                               camera_fog));
	}

	// Bind the camera's constant buffer
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<Pipeline>(device_context, slot);
	}


	//----------------------------------------------------------------------------------
	// Viewport
	//----------------------------------------------------------------------------------

	// Set a new viewport
	void setViewport(D3D11_VIEWPORT vp) noexcept {
		viewport.setViewport(std::move(vp));
		updateProjectionMatrix();
	}

	// Change the viewport size
	void resizeViewport(u32 width, u32 height) noexcept {
		viewport.setSize(width, height);
		updateProjectionMatrix();
	}

	void setViewportTopLeft(u32 x, u32 y) noexcept {
		viewport.setTopLeft(x, y);
	}

	void setViewportDepth(f32 min, f32 max) noexcept {
		viewport.setDepth(min, max);
	}


	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------

	// Set the depth range
	void setZDepth(f32 z_near, f32 z_far) {
		this->z_near = z_near;
		this->z_far  = z_far;
		updateProjectionMatrix();
	}


	//----------------------------------------------------------------------------------
	// Fog
	//----------------------------------------------------------------------------------

	// Set the fog buffer
	void setFog(const Fog& fog) {
		this->camera_fog = fog;
	}

	// Get the fog buffer
	[[nodiscard]]
	const Fog& getFog() const {
		return camera_fog;
	}


	//----------------------------------------------------------------------------------
	// Projection Matrix
	//----------------------------------------------------------------------------------

	// Get the camera's projection matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const {
		return projection_matrix;
	}


	//----------------------------------------------------------------------------------
	// Skybox
	//----------------------------------------------------------------------------------

	// Get the skybox associated with this camera
	[[nodiscard]]
	SkyBox& getSkybox() {
		return sky;
	}

	[[nodiscard]]
	const SkyBox& getSkybox() const {
		return sky;
	}


protected:
	// Update the projection matrix after changing depth/width/height/etc...
	virtual void updateProjectionMatrix() = 0;


protected:
	// The camera's constant buffer
	ConstantBuffer<CameraBuffer> buffer;

	// Viewport and z depth
	Viewport viewport;
	f32 z_near;
	f32 z_far;

	// The skybox for this camera
	SkyBox sky;

	// The fog that this camera sees
	Fog camera_fog;

	// Camera-to-projection matrix (view-to-projection)
	XMMATRIX projection_matrix;
};
