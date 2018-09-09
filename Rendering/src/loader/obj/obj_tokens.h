#pragma once

#include "string/string.h"

namespace ObjTokens {
	constexpr const wchar_t comment = L'#';

	constexpr gsl::cwzstring<> vertex          = L"v";
	constexpr gsl::cwzstring<> normal          = L"vn";
	constexpr gsl::cwzstring<> texture         = L"vt";
	constexpr gsl::cwzstring<> face            = L"f";
	constexpr gsl::cwzstring<> group           = L"g";
	constexpr gsl::cwzstring<> object          = L"o";
	constexpr gsl::cwzstring<> smoothing_group = L"s";

	constexpr gsl::cwzstring<> mtl_library = L"mtllib";
	constexpr gsl::cwzstring<> use_mtl     = L"usemtl";
	constexpr gsl::cwzstring<> new_mtl     = L"newmtl";

	constexpr gsl::cwzstring<> specular_exponent   = L"Ns";
	constexpr gsl::cwzstring<> specular_color      = L"Ks";
	constexpr gsl::cwzstring<> ambient_color       = L"Ka";
	constexpr gsl::cwzstring<> diffuse_color       = L"Kd";
	constexpr gsl::cwzstring<> emissive_color      = L"Ke";
	constexpr gsl::cwzstring<> optical_density     = L"Ni";
	constexpr gsl::cwzstring<> transparency        = L"Tr";
	constexpr gsl::cwzstring<> transparency_inv    = L"d";
	constexpr gsl::cwzstring<> transmission_filter = L"Tf";
	constexpr gsl::cwzstring<> illumination_model  = L"illum";

	constexpr gsl::cwzstring<> ambient_color_map  = L"map_Ka";
	constexpr gsl::cwzstring<> diffuse_color_map  = L"map_Kd";
	constexpr gsl::cwzstring<> specular_color_map = L"map_Ks";
	constexpr gsl::cwzstring<> spec_highlight_map = L"map_Ns";
	constexpr gsl::cwzstring<> alpha_texture_map  = L"map_d";
	constexpr gsl::cwzstring<> bump_map           = L"bump";
	constexpr gsl::cwzstring<> bump_map2          = L"map_bump";
};
