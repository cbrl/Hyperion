#include "stdafx.h"
#include "System.h"


System::System(int width, int height) {
	m_D3DApp = nullptr;

	m_Width  = width;
	m_Height = height;
}


System::~System() {
}


bool System::Init() {
	m_D3DApp = new Direct3D(m_Width, m_Height);

	if (!m_D3DApp->Init()) {
		return false;
	}

	return true;
}


bool System::Tick() {
	m_D3DApp->Frame();
}


