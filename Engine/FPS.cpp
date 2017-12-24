#include "stdafx.h"
#include "FPS.h"


FPS::FPS() : m_FPS(0), m_Count(0) {
	m_StartTime = timeGetTime();
}


FPS::~FPS() {
}


void FPS::Tick() {
	// Increment frame count
	m_Count++;

	//Set fps to 2x frame count when half a second has passed
	if (timeGetTime() >= (m_StartTime + 500)) {
		m_FPS = m_Count * 2;
		m_Count = 0;

		m_StartTime = timeGetTime();
	}
}


int FPS::GetFPS() {
	return m_FPS;
}
