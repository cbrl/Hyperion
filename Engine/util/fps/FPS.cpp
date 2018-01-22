#include "stdafx.h"
#include "FPS.h"


FPS::FPS() : fps(0), count(0) {
	startTime = timeGetTime();
}


FPS::~FPS() {
}


void FPS::Tick() {
	// Increment frame count
	count++;

	// Update FPS every half second
	if (timeGetTime() >= (startTime + 500)) {
		fps = count * 2;
		count = 0;

		startTime = timeGetTime();
	}
}


int FPS::GetFPS() {
	return fps;
}
