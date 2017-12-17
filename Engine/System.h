#pragma once

#include "Direct3D.h"

class System {
	public:
		System(int width, int height);
		~System();

		bool Init();
		bool Tick();

	private:
		Direct3D *m_D3DApp;
		int       m_Width;
		int       m_Height;
};
