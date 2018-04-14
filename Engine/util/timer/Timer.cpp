#include "stdafx.h"
#include "timer.h"


Timer::Timer() : stopped(false) {
	INT64 frequency;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	ticksPerMS = (double)(frequency / 1000);

	Reset();
}


void Timer::Tick() {
	if (stopped) {
		deltaTime = 0.0;
		return;
	}

	// Get current time
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	currTime = currentTime;

	// Calculate time since last tick
	deltaTime = (double)(currTime - prevTime);
	deltaTime /= ticksPerMS;

	// Store time of this tick
	prevTime = currTime;

	if (deltaTime < 0.0) {
		deltaTime = 0.0;
	}
}


float Timer::DeltaTime() {
	return (float)deltaTime;
}


float Timer::TotalTime() {
	// Return total time since the timer was started/reset, minus time spent paused.
	if (stopped) {
		return (float)(((stopTime - pausedTime) - baseTime)/ticksPerMS);
	}
	else {
		return (float)(((currTime - pausedTime) - baseTime)/ticksPerMS);
	}
}


void Timer::Start() {
	INT64 start_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&start_time);


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        start_time     

	if (stopped) {
		pausedTime += (start_time - stopTime);

		prevTime = start_time;
		stopTime = 0;
		stopped  = false;
	}
}


void Timer::Stop() {
	if (!stopped) {
		INT64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		stopTime = currTime;
		stopped = true;
	}
}


void Timer::Reset() {
	INT64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	baseTime = currTime;
	prevTime = currTime;
	stopTime = 0;
	stopped  = false;
}