#pragma once

#include "Direct3D.h"

class System {
	public:
		System(HWND hWnd, int windowWidth, int windowHeight);
		~System();

		bool Init();
		bool Tick();


	private:
		Direct3D *m_D3DApp;

		HWND      m_hWnd;
		int       m_WindowWidth;
		int       m_WindowHeight;
};
