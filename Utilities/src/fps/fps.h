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

	// Update the fps counter
	void tick() noexcept {
		frame_count++;

		// Update FPS when at least 1/2 second has passed
		if (SteadyClock::now() >= (start_time + 500ms)) {
			fps = frame_count * 2;
			frame_count = 0;

			start_time = SteadyClock::now();
		}
	}

	[[nodiscard]]
	u32 getFPS() const noexcept {
		return fps;
	}


private:
	u32 fps;
	u32 frame_count;
	SteadyClock::time_point start_time;
};
