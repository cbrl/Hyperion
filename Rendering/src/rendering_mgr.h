#pragma once

#include "direct3d/direct3d.h"
#include "render_state_mgr.h"
#include "resource/resource_mgr.h"
#include "renderer/renderer.h"


class RenderingMgr final {
public:
	RenderingMgr(HWND window, DisplayConfig config);
	~RenderingMgr();

	// Resize the viewport
	void resizeBuffers(u32 window_width, u32 window_height) const;

	// Bind the default RTV and DSV
	void bindDefaultRenderTarget() const {
		direct3D->bindDefaultRenderTarget();
	}

	// Render the current scene
	void render(Scene& scene);


	//----------------------------------------------------------------------------------
	// Getters
	//----------------------------------------------------------------------------------

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

	[[nodiscard]]
	const RenderStateMgr& getRenderStateMgr() const {
		return *render_state_mgr;
	}


private:
	// Start a new frame
	void beginFrame() const;

	// End the current frame
	void endFrame() const;


private:
	unique_ptr<Direct3D> direct3D;
	unique_ptr<RenderStateMgr> render_state_mgr;
	unique_ptr<ResourceMgr> resource_mgr;
	unique_ptr<Renderer> renderer;
};
