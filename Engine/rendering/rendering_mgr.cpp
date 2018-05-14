#include "stdafx.h"
#include "rendering_mgr.h"

#include "util\engine_util.h"
#include "engine\engine.h"


RenderingMgr::RenderingMgr(Engine& engine, bool fullscreen, bool vsync, bool msaa) {
	//----------------------------------------------------------------------------------
	// Initialize Direct3D
	//----------------------------------------------------------------------------------

	direct3D = make_unique<Direct3D>(engine.GetHWND(), engine.GetWindowWidth(), engine.GetWindowHeight(), fullscreen, vsync, msaa);
	FILE_LOG(logINFO) << "Initialized Direct3D";


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
	ImGui_ImplDX11_Init(engine.GetHWND(), &direct3D->GetDevice(), &direct3D->GetDeviceContext());
}


RenderingMgr::~RenderingMgr() {

	// Shutdown ImGui
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}


void RenderingMgr::Render(Engine& engine) const {

	// Start a new ImGui frame
	ImGui_ImplDX11_NewFrame();

	ImGui::ShowDemoWindow();


	// Clear the render target view with a specified color
	static float color[4] = { 0.39f, 0.39f, 0.39f, 1.0f };
	//ImGui::ColorEdit4("Clear Color", (float*)&color);

	direct3D->Clear(color);


	// Render the scene
	renderer->Render(engine);


	// Render the ImGui frame
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	// Present the final frame
	direct3D->PresentFrame();
}