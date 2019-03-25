#pragma once

#include <gsl/gsl>


namespace ConfigTokens {
	constexpr gsl::czstring<> win_title      = "window title";

	constexpr gsl::czstring<> display_config = "display configuration";
	constexpr gsl::czstring<> display_width  = "display width";
	constexpr gsl::czstring<> display_height = "display height";
	constexpr gsl::czstring<> refresh        = "refresh rate";
	constexpr gsl::czstring<> vsync          = "vsync";
	constexpr gsl::czstring<> fullscreen     = "fullscreen";
	constexpr gsl::czstring<> aa_type        = "anti aliasing";

	constexpr gsl::czstring<> render_config  = "rendering configuration";
	constexpr gsl::czstring<> shadowmap_res  = "shadow map resolution";
}