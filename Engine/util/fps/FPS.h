#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <timeapi.h>

class FPS {
	public:
		FPS();
		~FPS();

		void Tick();
		int  GetFPS();


	private:
		int   m_FPS;
		int   m_Count;
		ULONG m_StartTime;
};

