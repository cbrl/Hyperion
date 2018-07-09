#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "component/component.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "display/viewport.h"
#include "resource/texture/texture.h"


enum class RenderMode {
	Forward,
	ForwardPlus,
	Deferred
};

enum class LightingMode {
	Default,
	Unlit,
	FalseColorPosition,
	FalseColorNormal,
	FalseColorDepth,
};

enum class RenderOptions : u8 {
	None,
	BoundingVolume,
	Wireframe
};


class CameraSettings final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	CameraSettings()
		: render_mode(RenderMode::Forward)
		, lighting_mode(LightingMode::Default)
		, render_options(static_cast<u8>(RenderOptions::None)) {
	}

	CameraSettings(const CameraSettings& settings) noexcept = default;
	CameraSettings(CameraSettings&& settings) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~CameraSettings() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	CameraSettings& operator=(const CameraSettings& settings) noexcept = default;
	CameraSettings& operator=(CameraSettings&& settings) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Render Mode
	//----------------------------------------------------------------------------------

	RenderMode getRenderMode() const {
		return render_mode;
	}

	void setRenderMode(RenderMode mode) {
		render_mode = mode;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Lighting Mode
	//----------------------------------------------------------------------------------

	LightingMode getLightingMode() const {
		return lighting_mode;
	}

	void setLightingMode(LightingMode mode) {
		lighting_mode = mode;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Render Options
	//----------------------------------------------------------------------------------

	bool hasRenderOption(RenderOptions opt) const {
		return render_options & static_cast<u8>(opt);
	}

	void setRenderOption(RenderOptions opt) {
		render_options = static_cast<u8>(opt);
	}

	void toggleRenderOption(RenderOptions opt) {
		render_options ^= static_cast<u8>(opt);
	}

	void addRenderOption(RenderOptions opt) {
		render_options |= static_cast<u8>(opt);
	}

	void removeRenderOption(RenderOptions opt) {
		render_options &= ~(static_cast<u8>(opt));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Fog
	//----------------------------------------------------------------------------------

	Fog& getFog() {
		return fog;
	}

	const Fog& getFog() const {
		return fog;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Skybox
	//----------------------------------------------------------------------------------

	const Texture* getSkybox() const {
		return skybox.get();
	}

	void setSkybox(shared_ptr<Texture> sky) {
		skybox = std::move(sky);
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The rendering mode of the camera
	RenderMode render_mode;

	// The lighting mode of the camera
	LightingMode lighting_mode;

	// The extra rendering options
	u8 render_options;

	// Describes the fog color, start radius, range
	Fog fog;

	// The skybox texture
	shared_ptr<Texture> skybox;
};




template<typename T>
class CameraBase : public Component<T> {
	friend class CameraSystem;

protected:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	CameraBase(ID3D11Device& device)
		: buffer(device)
		, z_near(0.1f)
		, z_far(1000.0f)
		, projection_matrix(XMMatrixIdentity()) {

		viewport.setDepth(0.0f, 1.0f);
	}


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	CameraBase(const CameraBase& camera) = delete;
	CameraBase(CameraBase&& camera) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~CameraBase() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	CameraBase& operator=(const CameraBase& camera) = delete;
	CameraBase& operator=(CameraBase&& camera) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Misc
	//----------------------------------------------------------------------------------

	// Bind the viewport to the pipeline
	void bindViewport(ID3D11DeviceContext& device_context) const {
		viewport.bind(device_context);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Constant Buffer
	//----------------------------------------------------------------------------------

	// Update the camera's constant buffer
	void XM_CALLCONV updateBuffer(ID3D11DeviceContext& device_context,
	                              FXMMATRIX camera_to_world,
	                              CXMMATRIX world_to_camera) const {

		buffer.updateData(device_context,
						  CameraBuffer{XMMatrixTranspose(camera_to_world),
									   XMMatrixTranspose(world_to_camera),
									   XMMatrixTranspose(projection_matrix),
		                               settings.getFog()});
	}

	// Bind the camera's constant buffer
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<Pipeline>(device_context, slot);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Viewport
	//----------------------------------------------------------------------------------

	// Set a new viewport
	void setViewport(D3D11_VIEWPORT vp) noexcept {
		viewport.setViewport(std::move(vp));
	}

	// Change the viewport size
	void resizeViewport(u32 width, u32 height) noexcept {
		viewport.setSize(width, height);
	}

	void setViewportTopLeft(u32 x, u32 y) noexcept {
		viewport.setTopLeft(x, y);
	}

	void setViewportDepth(f32 min, f32 max) noexcept {
		viewport.setDepth(min, max);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Z Depth
	//----------------------------------------------------------------------------------

	// Set the depth range
	void setZDepth(f32 z_near, f32 z_far) {
		this->z_near = z_near;
		this->z_far  = z_far;
		updateProjectionMatrix();
	}

	void setZNear(f32 z_near) {
		this->z_near = z_near;
		updateProjectionMatrix();
	}

	void setZFar(f32 z_far) {
		this->z_far = z_far;
		updateProjectionMatrix();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Projection Matrix
	//----------------------------------------------------------------------------------

	// Get the camera's projection matrix
	[[nodiscard]]
	XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const {
		return projection_matrix;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Settings
	//----------------------------------------------------------------------------------

	CameraSettings& getSettings() {
		return settings;
	}

	const CameraSettings& getSettings() const {
		return settings;
	}


protected:
	// Update the projection matrix after changing depth/width/height/etc...
	virtual void updateProjectionMatrix() = 0;


protected:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The camera's constant buffer
	ConstantBuffer<CameraBuffer> buffer;

	// The camera's viewport description
	Viewport viewport;

	// Z Depth
	f32 z_near;
	f32 z_far;

	// Camera-to-projection matrix (view-to-projection)
	XMMATRIX projection_matrix;

	// Camera settings (render settings, fog, skybox)
	CameraSettings settings;
};
