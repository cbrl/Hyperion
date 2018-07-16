#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "os/windows/windows.h"


enum class AAType {
	none,
	msaa_2x,
	msaa_4x,
	msaa_8x,
};


class DisplayConfig final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	DisplayConfig() noexcept
		: curr_desc(0)
		, anti_aliasing(AAType::none)
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

	void setDisplayDesc(u32 index) {
		curr_desc = index;
	}

	[[nodiscard]]
	const vector<DXGI_MODE_DESC>& getDisplayDescList() const {
		return display_desc_list;
	}

	[[nodiscard]]
	DXGI_MODE_DESC getDisplayDesc() const {
		return display_desc_list[curr_desc];
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Anti Aliasing
	//----------------------------------------------------------------------------------

	void setAAType(AAType value) {
		anti_aliasing = value;
	}

	[[nodiscard]]
	AAType getAAType() const {
		return anti_aliasing;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Fullscreen
	//----------------------------------------------------------------------------------

	void setFullscreen(bool state) {
		fullscreen = state;
	}

	[[nodiscard]]
	bool isFullscreen() const {
		return fullscreen;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - VSync
	//----------------------------------------------------------------------------------

	void setVsync(bool state) {
		vsync = state;
	}

	[[nodiscard]]
	bool isVsync() const {
		return vsync;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Adapter/Output
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	IDXGIAdapter* getAdapter() const {
		return adapter.Get();
	}

	[[nodiscard]]
	IDXGIOutput* getOutput() const {
		return adapter_out.Get();
	}



private:
	void init() {
		ComPtr<IDXGIFactory> factory;

		// Create the DXGI factory
		ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), static_cast<void**>(&factory)),
			"Failed to create dxgiFactory");

		// Get the default adapter and output
		factory->EnumAdapters(0, &adapter);
		adapter->EnumOutputs(0, &adapter_out);

		// Get the number of display modes
		u32 mode_count;
		adapter_out->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &mode_count, nullptr);

		// Get the display modes
		vector<DXGI_MODE_DESC> display_modes(mode_count);
		adapter_out->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&mode_count,
			display_modes.data());


		// Store the display modes that are above 800px wide
		for (u32 i = 0; i < mode_count; i++) {
			if (display_modes[i].Width <= 800)
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

	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIOutput> adapter_out;
	vector<DXGI_MODE_DESC> display_desc_list;
	u32 curr_desc;
	AAType anti_aliasing;
	bool   fullscreen;
	bool   vsync;
};