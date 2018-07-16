#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "component/component.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "display/viewport.h"
#include "resource/texture/texture.h"


enum class RenderMode : u8 {
	Forward,
	ForwardPlus,
	Deferred
};

enum class LightingMode : u8 {
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
		, depth(0.01f, 100.0f) {

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
									   XMMatrixTranspose(getCameraToProjectionMatrix()),
		                               settings.getFog()});
	}

	// Bind the camera's constant buffer
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<Pipeline>(device_context, slot);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Viewport
	//----------------------------------------------------------------------------------

	Viewport& getViewport() {
		return viewport;
	}

	const Viewport& getViewport() const {
		return viewport;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Z Depth
	//----------------------------------------------------------------------------------

	vec2_f32 getZDepth() const {
		return depth;
	}

	void setZDepth(f32 z_near, f32 z_far) {
		depth.x = std::min(z_near, z_far - 0.001f);
		depth.y = std::max(z_far, z_near + 0.001f);
	}

	void setZDepth(vec2_f32 depth) {
		setZDepth(depth.x, depth.y);
	}

	void setZNear(f32 z_near) {
		depth.x = std::min(z_near, depth.y - 0.001f);
	}

	void setZFar(f32 z_far) {
		depth.y = std::max(z_far, depth.x + 0.001f);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Projection Matrix
	//----------------------------------------------------------------------------------

	// Get the camera's view-to-projection matrix
	[[nodiscard]]
	virtual XMMATRIX XM_CALLCONV getCameraToProjectionMatrix() const = 0;


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
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The camera's constant buffer
	ConstantBuffer<CameraBuffer> buffer;

	// The camera's viewport description
	Viewport viewport;

	// Z Depth
	vec2_f32 depth;

	// Camera settings (render settings, fog, skybox)
	CameraSettings settings;
};
