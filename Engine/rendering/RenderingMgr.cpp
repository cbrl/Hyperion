#include "stdafx.h"
#include "RenderingMgr.h"
#include "system\System.h"


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
	direct3D->Init();


	// Create render state manager and render states
	render_state_mgr = make_unique<RenderStateMgr>();


	// Initialize shader manager and shaders
	shader_mgr = make_unique<ShaderMgr>();
	shader_mgr->Init(hWnd, direct3D->GetDevice());


	// Create and bind buffers
	cbuffer_mgr = make_unique<CBufferMgr>();


	// Create texture manager
	texture_mgr = make_unique<TextureMgr>();


	// Create renderer
	renderer = make_unique<Renderer>();
}