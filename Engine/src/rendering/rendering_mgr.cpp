#include "stdafx.h"
#include "rendering_mgr.h"

#include "util/engine_util.h"
#include "engine/engine.h"


RenderingMgr::RenderingMgr(Engine& engine, bool fullscreen, bool vsync, bool msaa) {

	//----------------------------------------------------------------------------------
	// Initialize Direct3D
	//----------------------------------------------------------------------------------

	direct3D = make_unique<Direct3D>(engine.getHwnd(),
	                                 engine.getWindowWidth(),
	                                 engine.getWindowHeight(),
	                                 fullscreen,
	                                 vsync,
	                                 msaa);
	FILE_LOG(logINFO) << "Initialized Direct3D";


	//----------------------------------------------------------------------------------
	// Create render state manager and render states
	//----------------------------------------------------------------------------------

	render_state_mgr = make_unique<RenderStateMgr>(direct3D->getDevice(), direct3D->getDeviceContext());


	//----------------------------------------------------------------------------------
	// Create texture manager
	//----------------------------------------------------------------------------------

	resource_mgr = make_unique<ResourceMgr>(direct3D->getDevice(), direct3D->getDeviceContext());


	//----------------------------------------------------------------------------------
	// Initialize ImGui
	//----------------------------------------------------------------------------------

	ImGui::CreateContext();
	ImGui_ImplDX11_Init(engine.getHwnd(), &direct3D->getDevice(), &direct3D->getDeviceContext());
}


RenderingMgr::~RenderingMgr() {

	// Shutdown ImGui
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}


void RenderingMgr::resizeBuffers(u32 window_width, u32 window_height) const {
	direct3D->resizeBuffers(window_width, window_height);
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}


void RenderingMgr::beginFrame() const {

	// Start a new ImGui frame
	ImGui_ImplDX11_NewFrame();

	ImGui::ShowDemoWindow();


	// Clear the render target view with a specified color
	static float color[4] = {0.39f, 0.39f, 0.39f, 1.0f};
	//ImGui::ColorEdit4("Clear Color", (float*)&color);

	direct3D->clear(color);
}


void RenderingMgr::endFrame() const {

	// Render the ImGui frame
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present the final frame
	direct3D->presentFrame();
}
