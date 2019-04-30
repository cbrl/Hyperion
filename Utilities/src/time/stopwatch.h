#pragma once

#include "time/time.h"


template<typename ClockT = std::chrono::steady_clock>
class Stopwatch final {
public:
	using clock_t = ClockT;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Stopwatch();
	Stopwatch(const Stopwatch& timer) noexcept = default;
	Stopwatch(Stopwatch&& timer) noexcept = default;

	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Stopwatch() = default;

	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Stopwatch& operator=(const Stopwatch& timer) noexcept = default;
	Stopwatch& operator=(Stopwatch&& timer) noexcept = default;

	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Pause the timer
	void pause();

	// Start the timer (if it's paused)
	void resume();

	// Reset the timer
	void reset();

	// Update the timer
	void tick();

	// Get the time elapsed since the last update in the specified units (default: seconds)
	template<typename PeriodT = std::ratio<1>>
	[[nodiscard]]
	std::chrono::duration<f64, PeriodT> deltaTime() const;

	// Get the time elapsed since the timer was created or reset in the specified units (default: seconds)
	template<typename PeriodT = std::ratio<1>>
	[[nodiscard]]
	std::chrono::duration<f64, PeriodT> totalTime() const;

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	bool paused = false;

	using time_point = typename ClockT::time_point;
	using duration   = typename ClockT::duration;

	time_point base_time;
	time_point prev_time;
	duration   delta_time;
	duration   total_time;

	time_point pause_time;
	duration   pause_duration;
};

#include "stopwatch.tpp"