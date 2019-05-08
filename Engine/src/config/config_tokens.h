#pragma once

#include <gsl/gsl>


namespace ConfigTokens {
	// Window title
	constexpr gsl::czstring<> win_title = "window title";

	// Display config tokens
	constexpr gsl::czstring<> display_config = "display";
	constexpr gsl::czstring<> display_width  = "display width";
	constexpr gsl::czstring<> display_height = "display height";
	constexpr gsl::czstring<> refresh        = "refresh rate";
	constexpr gsl::czstring<> vsync          = "vsync";
	constexpr gsl::czstring<> fullscreen     = "fullscreen";
	constexpr gsl::czstring<> aa_type        = "anti aliasing";

	// Rendering config tokens
	constexpr gsl::czstring<> render_config                = "rendering";
	constexpr gsl::czstring<> smap_res                     = "shadow map resolution";
    constexpr gsl::czstring<> smap_depth_bias              = "shadow map depth bias";
    constexpr gsl::czstring<> smap_slope_scaled_depth_bias = "shadow map slope scaled depth bias";
    constexpr gsl::czstring<> smap_depth_bias_clamp        = "shadow map depth bias clamp";

	// Input config tokens
    constexpr gsl::czstring<> key_config = "input";
}