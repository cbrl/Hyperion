#include "timer.h"


Timer::Timer() : stopped(false) {
	INT64 frequency;

	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));

	ticks_per_ms = static_cast<double>(frequency) / 1000.0;

	reset();
}


void Timer::tick() {
	if (stopped) {
		delta_time = 0.0;
		return;
	}

	// Get current time
	INT64 current_time;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
	curr_time = current_time;

	// Calculate time since last tick
	delta_time = static_cast<double>(curr_time - prev_time);
	delta_time /= ticks_per_ms;

	// Store time of this tick
	prev_time = curr_time;

	if (delta_time < 0.0) {
		delta_time = 0.0;
	}
}


float Timer::deltaTime() const {
	return static_cast<float>(delta_time);
}


float Timer::totalTime() const {
	// Return total time since the timer was started/reset, minus time spent paused.
	if (stopped) {
		return static_cast<float>(((stop_time - paused_time) - base_time) / ticks_per_ms);
	}
	return static_cast<float>(((curr_time - paused_time) - base_time) / ticks_per_ms);
}


void Timer::start() {
	INT64 start_time;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_time));


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        start_time     

	if (stopped) {
		paused_time += (start_time - stop_time);

		prev_time = start_time;
		stop_time = 0;
		stopped = false;
	}
}


void Timer::stop() {
	if (!stopped) {
		INT64 curr_time;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr_time));

		stop_time = curr_time;
		stopped = true;
	}
}


void Timer::reset() {
	INT64 curr_time;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr_time));

	base_time = curr_time;
	prev_time = curr_time;
	stop_time = 0;
	stopped = false;
}
