#pragma once

#include <gsl/gsl>


namespace ConfigTokens {
	constexpr const char comment = '#';

	constexpr gsl::czstring<> width = "width";
	constexpr gsl::czstring<> height = "height";
	constexpr gsl::czstring<> refresh = "refresh";
	constexpr gsl::czstring<> vsync = "vsync";
	constexpr gsl::czstring<> fullscreen = "fullscreen";

	constexpr gsl::czstring<> win_title = "title";

	constexpr gsl::czstring<> shadowmap_res = "shadowmap_res";
}