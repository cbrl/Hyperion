#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <timeapi.h>
#include "datatypes/datatypes.h"


class FPS final {
public:
	FPS();
	FPS(const FPS& fps) = default;
	FPS(FPS&& fps) = default;
	~FPS() = default;

	void tick();
	u32 getFPS() const;


private:
	u32 fps;
	u32 count;
	ULONG start_time;
};
