#pragma once

#include "datatypes/datatypes.h"
#include "directx/d3d11.h"
#include "os/windows/windows.h"
#include "rendering_options.h"
#include "config/config_tokens.h"
#include "json/json.h"


namespace render {

class DisplayConfig final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	DisplayConfig() {
		init();
	}

	DisplayConfig(AAType aa,
	              bool fullscreen,
	              bool vsync)
		: anti_aliasing(aa)
		, fullscreen(fullscreen)
		, vsync(vsync) {

		init();
	}

	DisplayConfig(const DisplayConfig& config) = default;
	DisplayConfig(DisplayConfig&& config) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~DisplayConfig() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	DisplayConfig& operator=(const DisplayConfig& config) = default;
	DisplayConfig& operator=(DisplayConfig&& config) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Display Desc
	//----------------------------------------------------------------------------------

	void setDisplayDesc(size_t index) noexcept {
		curr_desc = index;
	}

	// Priority: format > res_width > res_height > refresh_rate
	void setNearestDisplayDesc(const u32_2& resolution,
	                           u32 refresh_rate = 0,
	                           DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);

	[[nodiscard]]
	size_t getDisplayDescIndex() const noexcept {
		return curr_desc;
	}

	[[nodiscard]]
	const std::vector<DXGI_MODE_DESC>& getDisplayDescList() const noexcept {
		return display_desc_list;
	}

	[[nodiscard]]
	const DXGI_MODE_DESC& getDisplayDesc() const noexcept {
		return display_desc_list[curr_desc];
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Resolution
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32 getDisplayWidth() const noexcept {
		return getDisplayDesc().Width;
	}

	[[nodiscard]]
	u32 getDisplayHeight() const noexcept {
		return getDisplayDesc().Height;
	}

	[[nodiscard]]
	u32_2 getDisplayResolution() const noexcept {
		return { getDisplayWidth(), getDisplayHeight() };
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Refresh Rate
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32 getRoundedDisplayRefreshRate() const noexcept {
		const auto numerator   = static_cast<f32>(getDisplayDesc().RefreshRate.Numerator);
		const auto denominator = static_cast<f32>(getDisplayDesc().RefreshRate.Denominator);
		return static_cast<u32>(round(numerator / denominator));
	}

	[[nodiscard]]
	DXGI_RATIONAL getDisplayRefreshRate() const noexcept {
		return getDisplayDesc().RefreshRate;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Display Format
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	DXGI_FORMAT getDisplayFormat() const noexcept {
		return getDisplayDesc().Format;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scaling
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	DXGI_MODE_SCALING getDisplayScaling() const noexcept {
		return getDisplayDesc().Scaling;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Scanline Ordering
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	DXGI_MODE_SCANLINE_ORDER getDisplayScanlineOrdering() const noexcept {
		return getDisplayDesc().ScanlineOrdering;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Anti Aliasing
	//----------------------------------------------------------------------------------

	void setAAType(AAType value) noexcept {
		anti_aliasing = value;
	}

	[[nodiscard]]
	AAType getAAType() const noexcept {
		return anti_aliasing;
	}

	[[nodiscard]]
	bool hasAA() const noexcept {
		return anti_aliasing != AAType::None;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Fullscreen
	//----------------------------------------------------------------------------------

	void setFullscreen(bool state) noexcept {
		fullscreen = state;
	}

	[[nodiscard]]
	bool isFullscreen() const noexcept {
		return fullscreen;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - VSync
	//----------------------------------------------------------------------------------

	void setVsync(bool state) noexcept {
		vsync = state;
	}

	[[nodiscard]]
	bool isVsync() const noexcept {
		return vsync;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Adapter/Output
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	IDXGIAdapter1* getAdapter() const noexcept {
		return adapter.Get();
	}

	[[nodiscard]]
	IDXGIOutput* getOutput() const noexcept {
		return adapter_out.Get();
	}


	//----------------------------------------------------------------------------------
	// Friend Functions - JSON Serialization
	//----------------------------------------------------------------------------------
	friend void to_json(json& j, const DisplayConfig& cfg);

	friend void from_json(const json& j, DisplayConfig& cfg);

private:

	//----------------------------------------------------------------------------------
	// Member Functions - Initialization
	//----------------------------------------------------------------------------------
	void init();


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIOutput1> adapter_out;

	std::vector<DXGI_MODE_DESC> display_desc_list;

	size_t curr_desc     = 0;
	AAType anti_aliasing = AAType::None;
	bool   fullscreen    = false;
	bool   vsync         = false;
};

} //namespace render
