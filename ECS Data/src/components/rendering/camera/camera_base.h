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
		, frustum(XMMatrixIdentity())
		, sky(device)
		, viewport({0.0f, 0.0f, 0.0f, 0.f, 0.0f, 1.0f})
		, z_near(0.1f)
		, z_far(1000.0f)
		, view_matrix(XMMatrixIdentity())
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
	                              FXMVECTOR position,
	                              FXMMATRIX world_to_camera) const {
		buffer.updateData(device_context,
		                  CameraBuffer(position,
		                               XMMatrixTranspose(world_to_camera),
		                               XMMatrixTranspose(projection_matrix),
		                               fog));
	}

	// Bind the camera's constant buffer
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<Pipeline>(device_context, slot);
	}


	//----------------------------------------------------------------------------------
	// Setters
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

	// Set the depth range
	void setZDepth(float z_near, float z_far) {
		this->z_near = z_near;
		this->z_far  = z_far;
		updateProjectionMatrix();
	}

	// Set the fog buffer
	void setFog(const Fog& fog) {
		this->fog = fog;
	}


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------

	// Get the camera's view matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getViewMatrix() const {
		return view_matrix;
	}

	// Get the camera's projection matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getProjectionMatrix() const {
		return projection_matrix;
	}

	// Get the Frustum for this camera
	[[nodiscard]]
	const Frustum& getFrustum() const {
		return frustum;
	}

	// Get the skybox associated with this camera
	[[nodiscard]]
	SkyBox& getSkybox() {
		return sky;
	}

	[[nodiscard]]
	const SkyBox& getSkybox() const {
		return sky;
	}

	// Get the fog buffer
	[[nodiscard]]
	const Fog& getFog() const {
		return fog;
	}


protected:
	// Update the projection matrix after changing depth/width/height/etc...
	virtual void updateProjectionMatrix() = 0;

	// Update the view matrix. Used after moving/rotating the camera.
	void XM_CALLCONV updateViewMatrix(FXMVECTOR position, FXMVECTOR forward, FXMVECTOR up) {
		// Create a target vector
		const XMVECTOR look_at = position + forward;

		// Create the new view matrix
		view_matrix = XMMatrixLookAtLH(position, look_at, up);

		// The frustum needs to be updated when the view matrix changes
		updateFrustum();
	}

	// Update the frustum. Used after the projection matrix or view matrix changes.
	void updateFrustum() {
		frustum.updateFrustum(view_matrix * projection_matrix);
	}


protected:
	// The camera's constant buffer
	ConstantBuffer<CameraBuffer> buffer;

	// Viewport and z depth
	D3D11_VIEWPORT viewport;
	float z_near;
	float z_far;

	// Camera frustum
	Frustum frustum;

	// The skybox for this camera
	SkyBox sky;

	// The fog that this camera sees
	Fog fog;

	// Matrices
	XMMATRIX view_matrix;
	XMMATRIX projection_matrix;

	// Default vectors
	static constexpr XMVECTOR default_forward = {0.0f, 0.0f, 1.0f, 0.0f};
	static constexpr XMVECTOR default_right   = {1.0f, 0.0f, 0.0f, 0.0f};
	static constexpr XMVECTOR default_up      = {0.0f, 1.0f, 0.0f, 0.0f};
};
