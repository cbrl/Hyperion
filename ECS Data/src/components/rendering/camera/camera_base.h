#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "component/component.h"
#include "pipeline.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "resource/resource_mgr.h"
#include "geometry/frustum/frustum.h"
#include "components/rendering/camera/skybox/skybox.h"


template<typename T>
class CameraBase : public Component<T> {
	friend class CameraSystem;

protected:
	CameraBase(ID3D11Device& device)
		: buffer(device)
		, viewport({0.0f, 0.0f, 0.0f, 0.f, 0.0f, 1.0f})
		, z_near(0.1f)
		, z_far(1000.0f)
		, sky(device)
		, projection_matrix(XMMatrixIdentity()) {
	}

	~CameraBase() = default;


public:
	// Bind the viewport to the pipeline
	void bindViewport(ID3D11DeviceContext& device_context) const {
		Pipeline::RS::bindViewports(device_context, 1, &viewport);
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
	void setViewport(ID3D11DeviceContext& device_context, const D3D11_VIEWPORT& viewport) {
		this->viewport = viewport;
		bindViewport(device_context);
		updateProjectionMatrix();
	}

	// Change the viewport size
	void resizeViewport(ID3D11DeviceContext& device_context, u32 width, u32 height) {
		viewport.Width  = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		bindViewport(device_context);
		updateProjectionMatrix();
	}

	void setViewportTopLeft(u32 top_left_x, u32 top_left_y) {
		viewport.TopLeftX = static_cast<float>(top_left_x);
		viewport.TopLeftY = static_cast<float>(top_left_y);
		updateProjectionMatrix();
	}


	//----------------------------------------------------------------------------------
	// Depth
	//----------------------------------------------------------------------------------

	// Set the depth range
	void setZDepth(float z_near, float z_far) {
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
	const Fog& fog() const {
		return camera_fog;
	}


	//----------------------------------------------------------------------------------
	// Projection Matrix
	//----------------------------------------------------------------------------------

	// Get the camera's projection matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV cameraToProjectionMatrix() const {
		return projection_matrix;
	}


	//----------------------------------------------------------------------------------
	// Skybox
	//----------------------------------------------------------------------------------

	// Get the skybox associated with this camera
	[[nodiscard]]
	SkyBox& skybox() {
		return sky;
	}

	[[nodiscard]]
	const SkyBox& skybox() const {
		return sky;
	}


protected:
	// Update the projection matrix after changing depth/width/height/etc...
	virtual void updateProjectionMatrix() = 0;


protected:
	// The camera's constant buffer
	ConstantBuffer<CameraBuffer> buffer;

	// Viewport and z depth
	D3D11_VIEWPORT viewport;
	float z_near;
	float z_far;

	// The skybox for this camera
	SkyBox sky;

	// The fog that this camera sees
	Fog camera_fog;

	// Camera-to-projection matrix (view-to-projection)
	XMMATRIX projection_matrix;
};
