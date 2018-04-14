#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <timeapi.h>

class FPS final {
	public:
		FPS();
		~FPS() = default;

		void Tick();
		u32  GetFPS();


	private:
		u32   fps;
		u32   count;
		ULONG start_time;
};

