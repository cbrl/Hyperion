#include "stdafx.h"
#include "EngineUtil.h"
#include "System.h"
#include "TextureMgr.h"
#include "Model.h"


System::System(HWND hWnd, int windowWidth, int windowHeight):
	m_hWnd(hWnd),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeight),
	m_D3DApp(nullptr)
{
}


System::~System() {
}


bool System::Init() {
	m_D3DApp = make_unique<Direct3D>(m_hWnd, m_WindowWidth, m_WindowHeight, false, true, false);

	if (!m_D3DApp->Init()) {
		return false;
	}
	
	auto textureMgr = make_unique<TextureMgr>(m_D3DApp->GetDevice().Get(), m_D3DApp->GetDeviceContext().Get());

	return true;
}


bool System::Tick() {
	
	return true;
}


void System::OnResize(int windowWidth, int windowHeight) {
	if (!this) return;
	if (m_D3DApp) {
		m_D3DApp->OnResize(windowWidth, windowHeight);
	}
}

