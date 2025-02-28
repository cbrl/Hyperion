module;

#include <DirectXMath.h>

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"

#include "directx/d3d11.h"

export module rendering:components.camera.camera_base;

import ecs;
import :constant_buffer;
import :buffer_types;
import :rendering_options;
import :texture;
import :viewport;

using namespace DirectX;

export namespace render {

class CameraSettings final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraSettings() noexcept = default;
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
	// Member Functions - BRDF
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	BRDF getBRDF() const noexcept {
		return brdf;
	}

	void setBRDF(BRDF type) noexcept {
		brdf = type;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - False Color
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	FalseColor getFalseColorMode() const noexcept {
		return false_color;
	}

	void setFalseColorMode(FalseColor type) noexcept {
		false_color = type;
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
	f32_4& getBoundingVolumeColor() noexcept{
		return bounding_volume_color;
	}

	[[nodiscard]]
	const f32_4& getBoundingVolumeColor() const noexcept{
		return bounding_volume_color;
	}

	void setBoundingVolumeColor(f32_4 color) noexcept {
		bounding_volume_color = std::move(color);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Wireframe Color
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	f32_4& getWireframeColor() noexcept{
		return wireframe_color;
	}

	[[nodiscard]]
	const f32_4& getWireframeColor() const noexcept{
		return wireframe_color;
	}

	void setWireframeColor(f32_4 color) noexcept {
		wireframe_color = std::move(color);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Fog
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	FogBuffer& getFog() noexcept {
		return fog;
	}

	[[nodiscard]]
	const FogBuffer& getFog() const noexcept {
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
	RenderMode render_mode = RenderMode::Forward;

	// The BRDF to use for lighting calculations
	BRDF brdf = BRDF::Microfacet;

	// The false color mode to use (when RenderMode::FalseColor is selected)
	FalseColor false_color = FalseColor::Fullbright;

	// The extra rendering options
	u8 render_options = static_cast<u8>(RenderOptions::None);

	// The color of bounding volumes and wireframes
	f32_4 bounding_volume_color = {1.0f, 1.0f, 1.0f, 1.0f};
	f32_4 wireframe_color = {0.0f, 1.0f, 0.0f, 1.0f};

	// Describes the fog color, start radius, range
	FogBuffer fog;

	// The skybox texture
	std::shared_ptr<Texture> skybox;
};




template<typename T>
class CameraBase : public ecs::Component {
protected:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	CameraBase(ID3D11Device& device)
		: buffer(device) {
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

		const XMMATRIX camera_to_projection = getCameraToProjectionMatrix();
		const u32_2    viewport_res         = viewport.getSize();

		CameraBuffer cam_buffer;
		cam_buffer.camera_to_world         = XMMatrixTranspose(camera_to_world);
		cam_buffer.world_to_camera         = XMMatrixTranspose(world_to_camera);
		cam_buffer.camera_to_projection    = XMMatrixTranspose(camera_to_projection);
		cam_buffer.projection_to_camera    = XMMatrixTranspose(XMMatrixInverse(nullptr, camera_to_projection));
		cam_buffer.viewport_top_left       = viewport.getTopLeft();
		cam_buffer.viewport_resolution     = viewport_res;
		cam_buffer.inv_viewport_resolution = {1.0f / viewport_res[0], 1.0f / viewport_res[1]};
		cam_buffer.fog                     = settings.getFog();

		buffer.updateData(device_context, cam_buffer);
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
	const f32_2& getZDepth() const noexcept{
		return depth;
	}

	void setZDepth(f32 z_near, f32 z_far) {
		depth[0] = std::min(z_near, z_far - 0.001f);
		depth[1] = std::max(z_far, z_near + 0.001f);
	}

	void setZDepth(const f32_2& depth) {
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
	f32_2 depth = {0.01f, 100.0f};

	// Camera settings (render settings, fog, skybox)
	CameraSettings settings;
};

} //namespace render