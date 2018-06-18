#pragma once

#include <chrono>
#include "datatypes/scalar_types.h"

using namespace std::chrono_literals;

class FPS final {
public:
	FPS();
	FPS(const FPS& fps) = default;
	FPS(FPS&& fps) = default;
	~FPS() = default;

	void tick();
	u32 getFPS() const;


private:
	using SteadyClock = std::chrono::steady_clock;

	u32 fps;
	u32 count;
	SteadyClock::time_point start_time;
};
