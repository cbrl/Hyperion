#include "stdafx.h"
#include "rendering_mgr.h"
#include "system\system.h"


const RenderingMgr* RenderingMgr::Get() {
	assert(System::Get());
	return System::Get()->GetRenderingMgr();
}


RenderingMgr::RenderingMgr(HWND window) : hWnd(window) {
}


RenderingMgr::~RenderingMgr() {
}


void RenderingMgr::Init(UINT window_width, UINT window_height, bool fullscreen, bool vsync, bool msaa) {

	// Initialize Direct3D
	direct3D = make_unique<Direct3D>(hWnd, window_width, window_height, fullscreen, vsync, msaa);


	// Create render state manager and render states
	render_state_mgr = make_unique<RenderStateMgr>();


	// Create texture manager
	texture_mgr = make_unique<TextureMgr>();


	// Create renderer
	renderer = make_unique<Renderer>();
}