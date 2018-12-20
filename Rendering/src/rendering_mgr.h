#pragma once

#include "rendering_config.h"
#include "direct3d/direct3d.h"
#include "direct3d/swapchain.h"
#include "resource/resource_mgr.h"
#include "renderer/renderer.h"


class RenderingMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	RenderingMgr(gsl::not_null<HWND> window,
	             DisplayConfig display_conf,
	             RenderingConfig rendering_conf);

	RenderingMgr(const RenderingMgr& mgr) = delete;
	RenderingMgr(RenderingMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~RenderingMgr();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	RenderingMgr& operator=(const RenderingMgr& mgr) = delete;
	RenderingMgr& operator=(RenderingMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Resizing
	//----------------------------------------------------------------------------------

	// Resize the buffers
	void onResize() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Rendering
	//----------------------------------------------------------------------------------

	// Start a new frame
	void beginFrame() const;

	// End the current frame
	void endFrame() const;

	// Render the current scene
	void render(Scene& scene) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Getters
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	DisplayConfig& getDisplayConfig() {
		return *display_config;
	}

	[[nodiscard]]
	const DisplayConfig& getDisplayConfig() const {
		return *display_config;
	}

	[[nodiscard]]
	RenderingConfig& getRenderingConfig() {
		return *rendering_config;
	}

	[[nodiscard]]
	const RenderingConfig& getRenderingConfig() const {
		return *rendering_config;
	}

	[[nodiscard]]
	ID3D11Device& getDevice() const {
		return direct3D->getDevice();
	}

	[[nodiscard]]
	ID3D11DeviceContext& getDeviceContext() const {
		return direct3D->getDeviceContext();
	}

	[[nodiscard]]
	SwapChain& getSwapChain() const {
		return *swap_chain;
	}

	[[nodiscard]]
	ResourceMgr& getResourceMgr() const {
		return *resource_mgr;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::unique_ptr<Direct3D>        direct3D;
	std::unique_ptr<DisplayConfig>   display_config;
	std::unique_ptr<RenderingConfig> rendering_config;
	std::unique_ptr<SwapChain>       swap_chain;
	std::unique_ptr<ResourceMgr>     resource_mgr;
	std::unique_ptr<Renderer>        renderer;
};
