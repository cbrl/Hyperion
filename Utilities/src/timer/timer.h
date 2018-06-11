#pragma once

#include "os/windows.h"


class Timer final {
public:
	Timer();
	~Timer() = default;

	void start();
	void stop();
	void reset();
	void tick();

	float deltaTime() const;
	float totalTime() const;


private:
	bool stopped;

	double ticks_per_ms;
	double delta_time;

	INT64 base_time;
	INT64 curr_time;
	INT64 prev_time;
	INT64 paused_time;
	INT64 stop_time;
};
