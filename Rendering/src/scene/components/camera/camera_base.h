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
	Deferred,
};

enum class LightingMode : u8 {
	Default,
	Unlit,
	FalseColorNormal,
	FalseColorDepth,
};

enum class RenderOptions : u8 {
	None           = 1,
	BoundingVolume = 1 << 1,
	Wireframe      = 1 << 2,
};


class CameraSettings final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	CameraSettings() noexcept
		: render_mode(RenderMode::Forward)
		, lighting_mode(LightingMode::Default)
		, render_options(static_cast<u8>(RenderOptions::None))
		, bounding_volume_color(1.0f, 1.0f, 1.0f, 1.0f)
		, wireframe_color(0.0f, 1.0f, 0.0f, 1.0f) {
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

	[[nodiscard]]
	RenderMode getRenderMode() const noexcept{
		return render_mode;
	}

	void setRenderMode(RenderMode mode) noexcept {
		render_mode = mode;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Lighting Mode
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	LightingMode getLightingMode() const noexcept{
		return lighting_mode;
	}

	void setLightingMode(LightingMode mode) noexcept {
		lighting_mode = mode;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Render Options
	//----------------------------------------------------------------------------------

	bool hasRenderOption(RenderOptions opt) const noexcept {
		return render_options & static_cast<u8>(opt);
	}

	void setRenderOption(RenderOptions opt) noexcept {
		render_options = static_cast<u8>(opt);
	}

	void toggleRenderOption(RenderOptions opt) noexcept {
		render_options ^= static_cast<u8>(opt);
	}

	void addRenderOption(RenderOptions opt) noexcept {
		render_options |= static_cast<u8>(opt);
	}

	void removeRenderOption(RenderOptions opt) noexcept {
		render_options &= ~(static_cast<u8>(opt));
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Bounding Volume Color
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	vec4_f32& getBoundingVolumeColor() noexcept{
		return bounding_volume_color;
	}

	[[nodiscard]]
	const vec4_f32& getBoundingVolumeColor() const noexcept{
		return bounding_volume_color;
	}

	void setBoundingVolumeColor(vec4_f32 color) noexcept {
		bounding_volume_color = std::move(color);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Wireframe Color
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	vec4_f32& getWireframeColor() noexcept{
		return wireframe_color;
	}

	[[nodiscard]]
	const vec4_f32& getWireframeColor() const noexcept{
		return wireframe_color;
	}

	void setWireframeColor(vec4_f32 color) noexcept {
		wireframe_color = std::move(color);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Fog
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	Fog& getFog() noexcept{
		return fog;
	}

	[[nodiscard]]
	const Fog& getFog() const noexcept {
		return fog;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Skybox
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const Texture* getSkybox() const noexcept {
		return skybox.get();
	}

	void setSkybox(std::shared_ptr<Texture> sky) {
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

	// The color of bounding volumes and wireframes
	vec4_f32 bounding_volume_color;
	vec4_f32 wireframe_color;

	// Describes the fog color, start radius, range
	Fog fog;

	// The skybox texture
	std::shared_ptr<Texture> skybox;
};




template<typename T>
class CameraBase : public Component<T> {
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

	[[nodiscard]]
	Viewport& getViewport() noexcept{
		return viewport;
	}

	[[nodiscard]]
	const Viewport& getViewport() const noexcept{
		return viewport;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Z Depth
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const vec2_f32& getZDepth() const noexcept{
		return depth;
	}

	void setZDepth(f32 z_near, f32 z_far) {
		depth[0] = std::min(z_near, z_far - 0.001f);
		depth[1] = std::max(z_far, z_near + 0.001f);
	}

	void setZDepth(const vec2_f32& depth) {
		setZDepth(depth[0], depth[1]);
	}

	void setZNear(f32 z_near) {
		depth[0] = std::min(z_near, depth[1] - 0.001f);
	}

	void setZFar(f32 z_far) {
		depth[1] = std::max(z_far, depth[0] + 0.001f);
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

	[[nodiscard]]
	CameraSettings& getSettings() noexcept{
		return settings;
	}

	[[nodiscard]]
	const CameraSettings& getSettings() const noexcept{
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
