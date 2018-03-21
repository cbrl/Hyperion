#include "stdafx.h"
#include "rendering_mgr.h"

#include "util\engine_util.h"
#include "system\system.h"


RenderingMgr::RenderingMgr(HWND hWnd, u32 window_width, u32 window_height, bool fullscreen, bool vsync, bool msaa) {
	//----------------------------------------------------------------------------------
	// Initialize Direct3D
	//----------------------------------------------------------------------------------

	direct3D = make_unique<Direct3D>(System::Get()->GetHWND(), window_width, window_height, fullscreen, vsync, msaa);


	//----------------------------------------------------------------------------------
	// Create render state manager and render states
	//----------------------------------------------------------------------------------

	render_state_mgr = make_unique<RenderStateMgr>(direct3D->GetDevice(), direct3D->GetDeviceContext());


	//----------------------------------------------------------------------------------
	// Create texture manager
	//----------------------------------------------------------------------------------

	resource_mgr = make_unique<ResourceMgr>(direct3D->GetDevice(), direct3D->GetDeviceContext());


	//----------------------------------------------------------------------------------
	// Create renderer
	//----------------------------------------------------------------------------------

	renderer = make_unique<Renderer>(direct3D->GetDevice(), direct3D->GetDeviceContext());


	//----------------------------------------------------------------------------------
	// Initialize ImGui
	//----------------------------------------------------------------------------------

	ImGui::CreateContext();
	ImGui_ImplDX11_Init(hWnd, direct3D->GetDevice(), direct3D->GetDeviceContext());
}


RenderingMgr::~RenderingMgr() {

	// Shutdown ImGui
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}


void RenderingMgr::Render(Scene& scene) const {

	// Start a new ImGui frame
	ImGui_ImplDX11_NewFrame();

	ImGui::ShowDemoWindow();


	// Clear the render target view with a specified color
	static float color[4] = { 0.39f, 0.39f, 0.39f, 1.0f };
	ImGui::ColorEdit4("Clear Color", (float*)&color);

	direct3D->Clear(color);


	// Render the scene
	renderer->Render(scene, *render_state_mgr);


	// Render the ImGui frame
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	// Present the final frame
	direct3D->PresentFrame();
}