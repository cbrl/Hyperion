#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "os/windows/windows.h"


enum class AAType {
	None,
	MSAA_2x,
	MSAA_4x,
	MSAA_8x,
};


class DisplayConfig final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	DisplayConfig() noexcept
		: curr_desc(0)
		, anti_aliasing(AAType::None)
		, fullscreen(false)
		, vsync(false) {

		init();
	}

	DisplayConfig(AAType aa,
	              bool fullscreen,
	              bool vsync)
		: curr_desc(0)
		, anti_aliasing(aa)
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

	[[nodiscard]]
	const std::vector<DXGI_MODE_DESC>& getDisplayDescList() const noexcept {
		return display_desc_list;
	}

	[[nodiscard]]
	DXGI_MODE_DESC getDisplayDesc() const noexcept {
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
	vec2_u32 getDisplayResolution() const noexcept {
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
		switch (anti_aliasing) {
			case AAType::MSAA_2x:
			case AAType::MSAA_4x:
			case AAType::MSAA_8x:
				return true;
			default:
				return false;
		}
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



private:
	void init() {
		ComPtr<IDXGIFactory2> factory;

		// Create the DXGI factory
		ThrowIfFailed(CreateDXGIFactory1(__uuidof(IDXGIFactory1), static_cast<void**>(&factory)),
			"Failed to create dxgiFactory");

		// Get the default adapter and output
		ComPtr<IDXGIOutput> output;
		factory->EnumAdapters1(0, adapter.ReleaseAndGetAddressOf());
		adapter->EnumOutputs(0, output.GetAddressOf());
		output.As(&adapter_out);

		// Get the number of display modes
		u32 mode_count;
		adapter_out->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &mode_count, nullptr);

		// Get the display modes
		std::vector<DXGI_MODE_DESC> display_modes(mode_count);
		adapter_out->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&mode_count,
			display_modes.data());


		// Discard the display modes that are below 800px wide or
		// have a scaling mode of DXGI_MODE_SCALING_STRETCHED
		for (u32 i = 0; i < mode_count; i++) {
			if (display_modes[i].Width <= 800)
				continue;
			if (display_modes[i].Scaling == DXGI_MODE_SCALING_STRETCHED)
				continue;

			display_desc_list.push_back(display_modes[i]);
		}


		// Get the highest refresh rate display mode for the current resolution
		for (u32 i = 0; i < display_desc_list.size(); ++i) {
			//if (display_desc_list[i].Width == GetSystemMetrics(SM_CXSCREEN) &&
			//	display_desc_list[i].Height == GetSystemMetrics(SM_CYSCREEN)) {
			if (display_desc_list[i].Width == display_desc_list[curr_desc].Width &&
				display_desc_list[i].Height == display_desc_list[curr_desc].Height) {

				const u32 new_rr = display_desc_list[i].RefreshRate.Numerator / display_desc_list[i].RefreshRate.Denominator;
				const u32 old_rr = display_desc_list[curr_desc].RefreshRate.Numerator / display_desc_list[curr_desc].RefreshRate.Denominator;

				if (new_rr > old_rr) curr_desc = i;
			}
		}
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIOutput1> adapter_out;
	std::vector<DXGI_MODE_DESC> display_desc_list;
	size_t curr_desc;
	AAType anti_aliasing;
	bool   fullscreen;
	bool   vsync;
};