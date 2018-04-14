#pragma once

#include <Windows.h>

class Timer final {
	public:
		Timer();
		~Timer() = default;

		void Start();
		void Stop();
		void Reset();
		void Tick();

		float DeltaTime();
		float TotalTime();


	private:
		bool stopped;

		double ticksPerMS;
		double deltaTime;

		INT64 baseTime;
		INT64 currTime;
		INT64 prevTime;
		INT64 pausedTime;
		INT64 stopTime;
};

