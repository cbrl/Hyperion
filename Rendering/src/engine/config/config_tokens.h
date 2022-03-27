#pragma once

#include <gsl/gsl>


namespace ConfigTokens {
	// Window title
	constexpr gsl::czstring<> win_title = "WindowTitle";

	// Display config tokens
	constexpr gsl::czstring<> display_config = "display";
	constexpr gsl::czstring<> display_width  = "Width";
	constexpr gsl::czstring<> display_height = "Height";
	constexpr gsl::czstring<> refresh        = "RefreshRate";
	constexpr gsl::czstring<> vsync          = "VSync";
	constexpr gsl::czstring<> fullscreen     = "Fullscreen";
	constexpr gsl::czstring<> aa_type        = "AntiAliasing";

	// Rendering config tokens
	constexpr gsl::czstring<> render_config                = "rendering";
	constexpr gsl::czstring<> smap_res                     = "ShadowMapResolution";
    constexpr gsl::czstring<> smap_depth_bias              = "ShadowMapDepthBias";
    constexpr gsl::czstring<> smap_slope_scaled_depth_bias = "ShadowMapSlopeScaledDepthBias";
    constexpr gsl::czstring<> smap_depth_bias_clamp        = "ShadowMapDepthBiasClamp";

	// Input config tokens
    constexpr gsl::czstring<> key_config = "input";
}