#include "stdafx.h"
#include "rendering_mgr.h"
#include "system\system.h"


RenderingMgr::RenderingMgr(u32 window_width, u32 window_height, bool fullscreen, bool vsync, bool msaa) {
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
}


void RenderingMgr::Render(Scene& scene) const {

	//----------------------------------------------------------------------------------
	// Clear the render target view with a specified color
	//----------------------------------------------------------------------------------

	float color[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	direct3D->Clear(color);


	//----------------------------------------------------------------------------------
	// Render the scene
	//----------------------------------------------------------------------------------

	renderer->Render(scene, *render_state_mgr);


	//----------------------------------------------------------------------------------
	// Present the frame
	//----------------------------------------------------------------------------------

	direct3D->PresentFrame();
}