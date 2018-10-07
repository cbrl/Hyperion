#pragma once

#include <chrono>
#include "datatypes/scalar_types.h"

using namespace std::chrono_literals;

class FPS final {
private:
	using SteadyClock = std::chrono::steady_clock;


public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	FPS() noexcept
		: fps(0)
		, frame_count(0)
		, wait_time(500ms)
		, start_time(SteadyClock::now()) {
	}

	FPS(const FPS& fps) noexcept = default;
	FPS(FPS&& fps) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~FPS() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	FPS& operator=(const FPS& fps) noexcept = default;
	FPS& operator=(FPS&& fps) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Update the FPS counter
	void tick() noexcept {
		frame_count++;

		// Update FPS when at least 1/2 second has passed
		if (SteadyClock::now() >= (start_time + wait_time)) {
			fps = static_cast<u32>(frame_count * (1000.0 / wait_time.count()));
			frame_count = 0;
			start_time = SteadyClock::now();
		}
	}

	// Set the time to wait before updating the FPS (in milliseconds)
	void setWaitTime(std::chrono::milliseconds time) noexcept {
		wait_time = time;
	}

	std::chrono::milliseconds getWaitTime() const noexcept {
		return wait_time;
	}

	[[nodiscard]]
	u32 getFPS() const noexcept {
		return fps;
	}


private:
	u32 fps;
	u32 frame_count;
	std::chrono::milliseconds wait_time;
	SteadyClock::time_point start_time;
};
