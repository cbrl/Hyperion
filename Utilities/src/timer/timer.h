#pragma once

#include <chrono>
#include <ratio>
#include "datatypes/scalar_types.h"


template<typename ClockT>
class Timer final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Timer();
	Timer(const Timer& timer) noexcept = default;
	Timer(Timer&& timer) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Timer() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Timer& operator=(const Timer& timer) noexcept = default;
	Timer& operator=(Timer&& timer) noexcept = default;


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
	f64 deltaTime() const;

	// Get the time elapsed since the timer was created or reset in the specified units (default: seconds)
	template<typename PeriodT = std::ratio<1>>
	[[nodiscard]]
	f64 totalTime() const;


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
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