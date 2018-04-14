#include "stdafx.h"
#include "fps.h"


FPS::FPS() : fps(0), count(0) {

	start_time = timeGetTime();
}



void FPS::Tick() {
	// Increment frame count
	count++;

	// Update FPS every half second
	if (timeGetTime() >= (start_time + 500)) {
		fps = count * 2;
		count = 0;

		start_time = timeGetTime();
	}
}


u32 FPS::GetFPS() {
	return fps;
}
