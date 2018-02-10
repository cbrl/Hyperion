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


void RenderingMgr::Init(UINT windowWidth, UINT windowHeight, bool fullscreen, bool vsync, bool msaa) {

	// Initialize Direct3D
	direct3D = make_unique<Direct3D>(hWnd, windowWidth, windowHeight, fullscreen, vsync, msaa);
	direct3D->Init();


	// Create render state manager and render states
	renderStateMgr = make_unique<RenderStateMgr>();
	renderStateMgr->SetupStates();


	// Initialize shader manager and shaders
	shaderMgr = make_unique<ShaderMgr>();
	shaderMgr->Init(hWnd);


	// Create and bind buffers
	cBufferMgr = make_unique<CBufferMgr>();


	// Create renderer
	renderer = make_unique<Renderer>();
}