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


bool RenderingMgr::Init(UINT windowWidth, UINT windowHeight, bool fullscreen, bool vsync, bool msaa) {

	// Initialize Direct3D
	direct3D = make_unique<Direct3D>(hWnd, windowWidth, windowHeight, fullscreen, vsync, msaa);
	if (!direct3D->Init()) {
		return false;
	}


	// Create render state manager and render states
	renderStateMgr = make_unique<RenderStateMgr>(direct3D->GetDevice(), direct3D->GetDeviceContext());
	renderStateMgr->SetupStates();


	// Initialize shader manager and shaders
	shaderMgr = make_unique<ShaderMgr>();
	if (!shaderMgr->Init(hWnd, direct3D->GetDevice().Get())) {
		return false;
	}


	// Create and bind buffers
	cBufferMgr = make_unique<CBufferMgr>(direct3D->GetDevice().Get(), direct3D->GetDeviceContext().Get());


	// Create renderer
	renderer = make_unique<Renderer>();


	return true;
}