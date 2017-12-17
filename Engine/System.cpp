#include "stdafx.h"
#include "System.h"


System::System(HWND hWnd, int windowWidth, int windowHeight)
:	m_hWnd(hWnd),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeight),
	m_D3DApp(nullptr)
{
}


System::~System() {
	SafeDelete(m_D3DApp);
}


bool System::Init() {
	m_D3DApp = new Direct3D(m_hWnd, m_WindowWidth, m_WindowHeight, false, false, false);

	if (!m_D3DApp->Init()) {
		return false;
	}

	return true;
}


bool System::Tick() {
	
	return true;
}


