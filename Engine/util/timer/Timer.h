#pragma once

#include <Windows.h>

class Timer {
	public:
		Timer();
		~Timer();

		void Start();
		void Stop();
		void Reset();
		void Tick();

		float DeltaTime();
		float TotalTime();


	private:
	bool m_Stopped;

	double m_TicksPerMS;
	double m_DeltaTime;

	INT64 m_BaseTime;
	INT64 m_CurrTime;
	INT64 m_PrevTime;
	INT64 m_PausedTime;
	INT64 m_StopTime;
};

