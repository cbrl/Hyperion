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


void DisplayConfig::setNearestDisplayDesc(const vec2_u32& resolution,
                                          u32 refresh_rate,
                                          DXGI_FORMAT format) {

	std::vector<std::vector<DXGI_MODE_DESC>::const_iterator> desc_matches;

	// Populate matches
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

	// Get best width
	{
		u32 closest_width = std::numeric_limits<u32>::max();

		for (const auto& desc : desc_matches) {
			const auto new_width = static_cast<i64>(desc->Width) - static_cast<i64>(resolution.x);
			const auto old_width = static_cast<i64>(closest_width) - static_cast<i64>(resolution.x);

			if (std::abs(new_width) < std::abs(old_width)) {
				closest_width = desc->Width;
			}
		}

		for (auto it = desc_matches.begin(); it != desc_matches.end();) {
			if ((*it)->Width != closest_width) {
				it = desc_matches.erase(it);
			}
			else {
				++it;
			}
		}
	}
	// Get best height
	{
		u32 closest_height = std::numeric_limits<u32>::max();

		for (const auto& desc : desc_matches) {
			const auto new_height = static_cast<i64>(desc->Height) - static_cast<i64>(resolution.y);
			const auto old_height = static_cast<i64>(closest_height) - static_cast<i64>(resolution.y);

			if (std::abs(new_height) < std::abs(old_height)) {
				closest_height = desc->Height;
			}
		}

		for (auto it = desc_matches.begin(); it != desc_matches.end();) {
			if ((*it)->Height != closest_height) {
				it = desc_matches.erase(it);
			}
			else {
				++it;
			}
		}
	}

	// Get best refresh rate
	if (refresh_rate != 0) {

		u32 closest_rr = std::numeric_limits<u32>::max();

		for (const auto& desc : desc_matches) {
			const auto num = static_cast<f32>(desc->RefreshRate.Numerator);
			const auto denom = static_cast<f32>(desc->RefreshRate.Denominator);
			const auto desc_rr = static_cast<i64>(round(num / denom));

			const auto new_rr = desc_rr - refresh_rate;
			const auto old_rr = static_cast<i64>(closest_rr - refresh_rate);
			if (std::llabs(new_rr) < std::llabs(old_rr)) {
				closest_rr = static_cast<u32>(desc_rr);
			}
		}
		for (auto it = desc_matches.begin(); it != desc_matches.end();) {
			const auto num = static_cast<f32>((*it)->RefreshRate.Numerator);
			const auto denom = static_cast<f32>((*it)->RefreshRate.Denominator);
			const auto desc_rr = static_cast<u32>(round(num / denom));
			if (desc_rr != closest_rr) {
				it = desc_matches.erase(it);
			}
			else {
				++it;
			}
		}
	}

	// Set the matching desc
	const size_t match_index = desc_matches[0] - display_desc_list.begin();
	setDisplayDesc(match_index);
}