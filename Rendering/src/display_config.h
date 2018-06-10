#pragma once

#include <d3d11.h>
#include "datatypes/scalar_types.h"


enum class AAType {
	none,
	msaa_2x,
	msaa_4x,
	msaa_8x,
};


class DisplayConfig final {
public:
	DisplayConfig()
		: width(800)
		, height(600)
		, refresh_rate(60)
		, anti_aliasing(AAType::none)
		, fullscreen(false)
		, vsync(false) {
	}

	DisplayConfig(u32 width,
	              u32 height,
	              u32 refresh,
	              AAType aa,
	              bool fullscreen,
	              bool vsync)
		: width(width)
		, height(height)
		, refresh_rate(refresh)
		, anti_aliasing(aa)
		, fullscreen(fullscreen)
		, vsync(vsync) {
	}

	DisplayConfig(const DisplayConfig& config) = default;
	DisplayConfig(DisplayConfig&& config) = default;
	~DisplayConfig() = default;


	void setWidth(u32 value) { width = value; }
	void setHieght(u32 value) { width = value; }
	void setRefreshRate(u32 value) { refresh_rate = value; }
	void setAAType(AAType value) { anti_aliasing = value; }
	void setFullscreen(bool state) { fullscreen = state; }
	void setVsync(bool state) { vsync = state; }


	u32 getWidth() const { return width; }
	u32 getHeight() const { return height; }
	u32 getRefreshRate() const { return refresh_rate; }
	AAType getAAType() const { return anti_aliasing; }
	bool isFullscreen() const { return fullscreen; }
	bool isVsync() const { return vsync; }


private:
	//DXGI_MODE_DESC display_desc;
	u32    width;
	u32    height;
	u32    refresh_rate;
	AAType anti_aliasing;
	bool   fullscreen;
	bool   vsync;
};