#include "rendering_mgr.h"
#include "log/log.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


RenderingMgr::RenderingMgr(HWND window, DisplayConfig config) {

	//----------------------------------------------------------------------------------
	// Initialize Direct3D
	//----------------------------------------------------------------------------------
	direct3D = make_unique<Direct3D>(window, config);
	Logger::log(LogLevel::info, "Initialized Direct3D");


	//----------------------------------------------------------------------------------
	// Create the render state manager and render states
	//----------------------------------------------------------------------------------
	render_state_mgr = make_unique<RenderStateMgr>(direct3D->getDevice(), direct3D->getDeviceContext());


	//----------------------------------------------------------------------------------
	// Create the resource manager
	//----------------------------------------------------------------------------------
	resource_mgr = make_unique<ResourceMgr>(direct3D->getDevice(), direct3D->getDeviceContext());


	//----------------------------------------------------------------------------------
	// Create the renderer
	//----------------------------------------------------------------------------------
	renderer = make_unique<Renderer>(direct3D->getDevice(), direct3D->getDeviceContext(), *render_state_mgr, *resource_mgr);


	//----------------------------------------------------------------------------------
	// Initialize ImGui
	//----------------------------------------------------------------------------------
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(&direct3D->getDevice(), &direct3D->getDeviceContext());
	ImGui::StyleColorsClassic();
}


RenderingMgr::~RenderingMgr() {

	// Shutdown ImGui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


void RenderingMgr::resizeBuffers(u32 window_width, u32 window_height) const {
	ImGui_ImplDX11_InvalidateDeviceObjects();
	direct3D->resizeBuffers(window_width, window_height);
	ImGui_ImplDX11_CreateDeviceObjects();
}


void RenderingMgr::render(const Engine& engine) {
	
	beginFrame();

	renderer->render(engine);

	endFrame();
}


void RenderingMgr::beginFrame() const {

	// Start a new ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();


	// Clear the render target view with a specified color
	static f32 color[4] = {0.39f, 0.39f, 0.39f, 1.0f};
	//ImGui::ColorEdit4("Clear Color", (f32*)&color);

	direct3D->clear(color);
}


void RenderingMgr::endFrame() const {

	// Render the ImGui frame
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present the final frame
	direct3D->presentFrame();
}
