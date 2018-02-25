#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <timeapi.h>

class FPS {
	public:
		FPS();
		~FPS();

		void Tick();
		i32  GetFPS();


	private:
		i32   fps;
		i32   count;
		ULONG start_time;
};

