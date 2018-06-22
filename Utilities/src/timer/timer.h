#pragma once

#include <chrono>
#include <ratio>
#include <thread> //std::thread::hardware_concurrency()
#include "datatypes/scalar_types.h"


template<typename ClockT>
class Timer final {
public:
	Timer();
	~Timer() = default;

	// Pause the timer
	void pause();

	// Start the timer (if it's paused)
	void resume();

	// Reset the timer
	void reset();

	// Update the timer
	void tick();

	// Get the time elapsed since the last update (in seconds)
	[[nodiscard]]
	double deltaTime() const;

	// Get the time elapsed since the timer was created or reset (in seconds)
	[[nodiscard]]
	double totalTime() const;


private:
	bool paused;

	using time_point = typename ClockT::time_point;
	using duration   = typename ClockT::duration;

	time_point base_time;
	time_point prev_time;
	duration delta_time;
	duration total_time;

	time_point pause_time;
	duration   pause_duration;
};

#include "timer.tpp"


using HighResTimer = Timer<std::chrono::high_resolution_clock>;