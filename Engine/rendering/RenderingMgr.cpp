#include "stdafx.h"
#include "RenderingMgr.h"
#include "system\System.h"


const RenderingMgr* RenderingMgr::Get() {
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


	// Initialize render state manager
	renderStateMgr = make_unique<RenderStateMgr>(hWnd, direct3D->GetDevice(), direct3D->GetDeviceContext());
	if (!renderStateMgr->Init()) {
		return false;
	}


	// Create and bind buffers
	cBufferMgr = make_unique<CBufferMgr>(direct3D->GetDevice().Get(), direct3D->GetDeviceContext().Get());


	// Create renderer
	renderer = make_unique<Renderer>();


	return true;
}