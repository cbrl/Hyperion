#pragma once


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

// Exclude rarely used stuff from windows headers
#define WIN32_LEAN_AND_MEAN

// Don't define the windows min/max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Silence warning for codecvt deprecation
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif




//----------------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------------

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Windows.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "exception/exception.h"
#include "datatypes/scalar_types.h"
#include "string/string.h"




//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

inline void ThrowIfFailed(HRESULT hr, const char* msg = "") {
	if (FAILED(hr)) {
		std::cerr << msg << " (Failure with HRESULT of 0x" << std::hex << hr << std::dec << ")\n";
		throw Exception(msg);
	}
}