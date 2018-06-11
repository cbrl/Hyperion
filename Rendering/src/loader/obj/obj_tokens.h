#pragma once

namespace ObjTokens {
	constexpr const wchar_t comment = L'#';

	constexpr const wchar_t* vertex          = L"v";
	constexpr const wchar_t* normal          = L"vn";
	constexpr const wchar_t* texture         = L"vt";
	constexpr const wchar_t* face            = L"f";
	constexpr const wchar_t* group           = L"g";
	constexpr const wchar_t* object          = L"o";
	constexpr const wchar_t* smoothing_group = L"s";

	constexpr const wchar_t* mtl_library = L"mtllib";
	constexpr const wchar_t* use_mtl     = L"usemtl";
	constexpr const wchar_t* new_mtl     = L"newmtl";

	constexpr const wchar_t* specular_exponent   = L"Ns";
	constexpr const wchar_t* specular_color      = L"Ks";
	constexpr const wchar_t* ambient_color       = L"Ka";
	constexpr const wchar_t* diffuse_color       = L"Kd";
	constexpr const wchar_t* emissive_color      = L"Ke";
	constexpr const wchar_t* optical_density     = L"Ni";
	constexpr const wchar_t* transparency        = L"Tr";
	constexpr const wchar_t* transparency_inv    = L"d";
	constexpr const wchar_t* transmission_filter = L"Tf";
	constexpr const wchar_t* illumination_model  = L"illum";

	constexpr const wchar_t* ambient_color_map  = L"map_Ka";
	constexpr const wchar_t* diffuse_color_map  = L"map_Kd";
	constexpr const wchar_t* specular_color_map = L"map_Ks";
	constexpr const wchar_t* spec_highlight_map = L"map_Ns";
	constexpr const wchar_t* alpha_texture_map  = L"map_d";
	constexpr const wchar_t* bump_map           = L"bump";
	constexpr const wchar_t* bump_map2          = L"map_bump";
};
