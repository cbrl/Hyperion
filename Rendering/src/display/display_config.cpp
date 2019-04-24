#include "display_config.h"


void DisplayConfig::init() {

	ComPtr<IDXGIFactory2> factory;

	// Create the DXGI factory
	ThrowIfFailed(CreateDXGIFactory1(__uuidof(IDXGIFactory2),
	                                 reinterpret_cast<void**>(factory.GetAddressOf())),
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
	for (size_t i = 0; i < mode_count; i++) {
		if (display_modes[i].Width <= 800)
			continue;
		if (display_modes[i].Scaling == DXGI_MODE_SCALING_STRETCHED)
			continue;

		display_desc_list.push_back(display_modes[i]);
	}


	// Get the highest refresh rate display mode for the current resolution
	for (size_t i = 0; i < display_desc_list.size(); ++i) {

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


void DisplayConfig::setNearestDisplayDesc(const u32_2& resolution,
                                          u32 refresh_rate,
                                          DXGI_FORMAT format) {

	using display_desc_list_iter = decltype(display_desc_list)::const_iterator;

	std::vector<display_desc_list_iter> desc_matches;

	//----------------------------------------------------------------------------------
	// Populate matches
	//----------------------------------------------------------------------------------
	if (format != DXGI_FORMAT_UNKNOWN) {
		for (auto it = display_desc_list.begin(); it != display_desc_list.end(); ++it) {
			if (it->Format == format)
				desc_matches.push_back(it);
		}
	}
	if (desc_matches.empty()) {
		for (auto it = display_desc_list.begin(); it != display_desc_list.end(); ++it) {
			desc_matches.push_back(it);
		}
	}

	//----------------------------------------------------------------------------------
	// Get best width
	//----------------------------------------------------------------------------------
	{
		// Sort the array by width (lowest -> highest)
		std::sort(desc_matches.begin(), desc_matches.end(),
			[](const display_desc_list_iter& a, const display_desc_list_iter& b) {
				return a->Width < b->Width;
		});

		// Find and remove any elements with width > desired_width
		auto greater = std::upper_bound(desc_matches.begin(), desc_matches.end(), resolution[0],
			[](u32 a, display_desc_list_iter& b) {
				return a < b->Width;
		});
		if (greater != desc_matches.end()) {
			desc_matches.erase(greater, desc_matches.end());
		}

		// Find and remove any elements with width != closest_match (aka matches.back())
		auto first_eq = std::find_if(desc_matches.begin(), desc_matches.end(),
			[&](const display_desc_list_iter& a) {
				return a->Width == desc_matches.back()->Width;
		});
		if (first_eq != desc_matches.end()) {
			desc_matches.erase(desc_matches.begin(), first_eq);
		}
	}

	//----------------------------------------------------------------------------------
	// Get best height
	//----------------------------------------------------------------------------------
	{
		// Sort the array by height (lowest -> highest)
		std::sort(desc_matches.begin(), desc_matches.end(),
			[](const display_desc_list_iter& a, const display_desc_list_iter& b) {
				return a->Height < b->Height;
		});

		// Find and remove any elements with height > desired_height
		auto greater = std::upper_bound(desc_matches.begin(), desc_matches.end(), resolution[1],
			[](u32 a, const display_desc_list_iter& b) {
				return a < b->Height;
		});
		if (greater != desc_matches.end()) {
			desc_matches.erase(greater, desc_matches.end());
		}

		// Find and remove any elements with height != closest_match (aka matches.back())
		auto first_eq = std::find_if(desc_matches.begin(), desc_matches.end(),
			[&](const display_desc_list_iter& a) {
				return a->Height == desc_matches.back()->Height;
		});
		if (first_eq != desc_matches.end()) {
			desc_matches.erase(desc_matches.begin(), first_eq);
		}
	}

	//----------------------------------------------------------------------------------
	// Get best refresh rate
	//----------------------------------------------------------------------------------
	if (refresh_rate != 0) {

		const auto rounded_refresh_rate = [](const DXGI_MODE_DESC& desc) -> i64 {
			const auto num   = static_cast<f32>(desc.RefreshRate.Numerator);
			const auto denom = static_cast<f32>(desc.RefreshRate.Denominator);
			return static_cast<i64>(round(num / denom));
		};

		// Sort the array by refresh rate (lowest -> highest)
		std::sort(desc_matches.begin(), desc_matches.end(),
			[&](const display_desc_list_iter& a, const display_desc_list_iter& b) {
				const auto a_rr = rounded_refresh_rate(*a);
			    const auto b_rr = rounded_refresh_rate(*b);

				return a_rr < b_rr;
		});

		// Find and remove any elements with refresh > desired_refresh
		auto greater = std::upper_bound(desc_matches.begin(), desc_matches.end(), refresh_rate,
			[&](u32 a, const display_desc_list_iter& b) {
			    const auto b_rr = rounded_refresh_rate(*b);

				return a < b_rr;
		});
		if (greater != desc_matches.end()) {
			desc_matches.erase(greater, desc_matches.end());
		}

		// Find and remove any elements with refresh rate != closest_match (aka matches.last())
		auto first_eq = std::find_if(desc_matches.begin(), desc_matches.end(),
			[&](const display_desc_list_iter& a) {
				const auto a_rr = rounded_refresh_rate(*a);
			    const auto b_rr = rounded_refresh_rate(*desc_matches.back());

				return a_rr == b_rr;
		});
		if (first_eq != desc_matches.end()) {
			desc_matches.erase(desc_matches.begin(), first_eq);
		}
	}

	// Set the matching desc
	if (desc_matches.empty()) {
		setDisplayDesc(0);
	}
	else {
		const size_t match_index = desc_matches[0] - display_desc_list.begin();
		setDisplayDesc(match_index);
	}
}


void to_json(json& j, const DisplayConfig& cfg) {
	const auto res = cfg.getDisplayResolution();
	j[ConfigTokens::display_width] = res[0];
	j[ConfigTokens::display_height] = res[1];
	j[ConfigTokens::refresh] = cfg.getRoundedDisplayRefreshRate();
	j[ConfigTokens::vsync] = cfg.isVsync();
	j[ConfigTokens::fullscreen] = cfg.isFullscreen();
	j[ConfigTokens::aa_type] = cfg.getAAType();
}


void from_json(const json& j, DisplayConfig& cfg) {
	u32_2 res;
	j.at(ConfigTokens::display_width).get_to(res[0]);
	j.at(ConfigTokens::display_height).get_to(res[1]);

	const auto refresh = j.at(ConfigTokens::refresh).get<u32>();
	const auto vsync = j.at(ConfigTokens::vsync).get<bool>();
	const auto fullscreen = j.at(ConfigTokens::fullscreen).get<bool>();
	const auto aa = static_cast<AAType>(j.at(ConfigTokens::aa_type).get<u32>());

	cfg.setNearestDisplayDesc(res, refresh);
	cfg.setVsync(vsync);
	cfg.setFullscreen(fullscreen);
	cfg.setAAType(aa);
}