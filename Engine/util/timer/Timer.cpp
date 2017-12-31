#include "stdafx.h"
#include "Timer.h"


Timer::Timer() : m_Stopped(false) {
	INT64 frequency;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	m_TicksPerMS = (double)(frequency / 1000);

	Reset();
}


Timer::~Timer() {
}


void Timer::Tick() {
	if (m_Stopped) {
		m_DeltaTime = 0.0;
		return;
	}

	// Get current time
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	m_CurrTime = currentTime;

	// Calculate time since last tick
	m_DeltaTime = (double)(m_CurrTime - m_PrevTime);
	m_DeltaTime /= m_TicksPerMS;

	// Store time of this tick
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0) {
		m_DeltaTime = 0.0;
	}
}


float Timer::DeltaTime() {
	return (float)m_DeltaTime;
}


float Timer::TotalTime() {
	// Return total time since the timer was started/reset, minus time spent paused.
	if (m_Stopped) {
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime)/m_TicksPerMS);
	}
	else {
		return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime)/m_TicksPerMS);
	}
}


void Timer::Start() {
	INT64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	if (m_Stopped) {
		m_PausedTime += (startTime - m_StopTime);

		m_PrevTime = startTime;
		m_StopTime = 0;
		m_Stopped  = false;
	}
}


void Timer::Stop() {
	if (!m_Stopped) {
		INT64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_StopTime = currTime;
		m_Stopped = true;
	}
}


void Timer::Reset() {
	INT64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_Stopped  = false;
}