#pragma once

#include "string/string.h"

namespace MtlTokens {

	constexpr const char comment = '#';

	constexpr gsl::czstring<> new_mtl = "newmtl";

	constexpr gsl::czstring<> specular_exponent = "Ns";
	constexpr gsl::czstring<> specular_color = "Ks";
	constexpr gsl::czstring<> ambient_color = "Ka";
	constexpr gsl::czstring<> diffuse_color = "Kd";
	constexpr gsl::czstring<> emissive_color = "Ke";
	constexpr gsl::czstring<> optical_density = "Ni";
	constexpr gsl::czstring<> transparency = "Tr";
	constexpr gsl::czstring<> transparency_inv = "d";
	constexpr gsl::czstring<> transmission_filter = "Tf";
	constexpr gsl::czstring<> illumination_model = "illum";

	constexpr gsl::czstring<> ambient_color_map = "map_Ka";
	constexpr gsl::czstring<> diffuse_color_map = "map_Kd";
	constexpr gsl::czstring<> specular_color_map = "map_Ks";
	constexpr gsl::czstring<> spec_highlight_map = "map_Ns";
	constexpr gsl::czstring<> alpha_texture_map = "map_d";
	constexpr gsl::czstring<> bump_map = "bump";
	constexpr gsl::czstring<> bump_map2 = "map_bump";
}