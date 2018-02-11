#pragma once

namespace OBJTokens {
	const wchar_t  comment     = L'#';

	const wchar_t* vertex      = L"v";
	const wchar_t* normal      = L"vn";
	const wchar_t* texture     = L"vt";
	const wchar_t* face        = L"f";
	const wchar_t* group       = L"g";
	const wchar_t* object      = L"o";
	const wchar_t* smoothGroup = L"s";

	const wchar_t* mtl_library = L"mtllib";
	const wchar_t* group_mtl   = L"usemtl";
	const wchar_t* new_mtl     = L"newmtl";

	const wchar_t* specular_exponent   = L"Ns";
	const wchar_t* specular_color      = L"Ks";
	const wchar_t* ambient_color       = L"Ka";
	const wchar_t* diffuse_color       = L"Kd";
	const wchar_t* emmisive_color      = L"Ke";
	const wchar_t* optical_density     = L"Ni";
	const wchar_t* transparency        = L"Tr";
	const wchar_t* transparency_inv    = L"d";
	const wchar_t* transmission_filter = L"Tf";
	const wchar_t* illumination_model  = L"illum";

	const wchar_t* ambient_color_map  = L"map_Ka";
	const wchar_t* diffuse_color_map  = L"map_Kd";
	const wchar_t* specular_color_map = L"map_Ks";
	const wchar_t* spec_highlight_map = L"map_Ns";
	const wchar_t* alpha_texture_map  = L"map_d";
	const wchar_t* bump_map           = L"bump";
	const wchar_t* bump_map2          = L"map_bump";
};