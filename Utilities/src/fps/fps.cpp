#include "fps.h"


FPS::FPS() : fps(0), count(0) {

	start_time = SteadyClock::now();
}


void FPS::tick() {
	// Increment frame count
	count++;

	// Update FPS every half second
	if (SteadyClock::now() >= (start_time + 500ms)) {
		fps = count * 2;
		count = 0;

		start_time = SteadyClock::now();
	}
}


u32 FPS::getFPS() const {
	return fps;
}
