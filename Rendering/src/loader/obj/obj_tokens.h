#pragma once

#include "string/string.h"

namespace ObjTokens {

	constexpr const char comment = '#';

	constexpr gsl::czstring<> vertex          = "v";
	constexpr gsl::czstring<> normal          = "vn";
	constexpr gsl::czstring<> texture         = "vt";
	constexpr gsl::czstring<> face            = "f";
	constexpr gsl::czstring<> group           = "g";
	constexpr gsl::czstring<> object          = "o";
	constexpr gsl::czstring<> smoothing_group = "s";

	constexpr gsl::czstring<> mtl_library = "mtllib";
	constexpr gsl::czstring<> use_mtl     = "usemtl";
};
