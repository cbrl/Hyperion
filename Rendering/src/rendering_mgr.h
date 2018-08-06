#pragma once

#include "direct3d/direct3d.h"
#include "direct3d/swapchain.h"
#include "resource/resource_mgr.h"
#include "renderer/renderer.h"


class RenderingMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	RenderingMgr(HWND window, DisplayConfig config);
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
	// Member Functions
	//----------------------------------------------------------------------------------

	// Resize the buffers
	void onResize() const;

	// Render the current scene
	void render(Scene& scene);

	[[nodiscard]]
	ID3D11Device& getDevice() const {
		return direct3D->getDevice();
	}

	[[nodiscard]]
	ID3D11DeviceContext& getDeviceContext() const {
		return direct3D->getDeviceContext();
	}

	[[nodiscard]]
	ResourceMgr& getResourceMgr() const {
		return *resource_mgr;
	}


private:
	// Start a new frame
	void beginFrame() const;

	// End the current frame
	void endFrame() const;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	unique_ptr<DisplayConfig> display_config;
	unique_ptr<Direct3D> direct3D;
	unique_ptr<SwapChain> swap_chain;
	unique_ptr<ResourceMgr> resource_mgr;
	unique_ptr<Renderer> renderer;
};
