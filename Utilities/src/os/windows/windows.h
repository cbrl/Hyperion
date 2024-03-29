#pragma once


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

// Exclude rarely used stuff from windows headers
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

// Don't define the windows min/max macros
#undef NOMINMAX
#define NOMINMAX

// Silence warning for codecvt deprecation
#undef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

// Declare std::aligned_storage behavior for MSVC
#undef _ENABLE_EXTENDED_ALIGNED_STORAGE
#define _ENABLE_EXTENDED_ALIGNED_STORAGE


//----------------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------------

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Windows.h>
#include <WinUser.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "exception/exception.h"
#include "datatypes/types.h"




//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

inline void ThrowIfFailed(HRESULT hr, const char* msg = "Unkown Error", const std::source_location& loc = std::source_location::current()) {
	if (FAILED(hr)) {
		std::stringstream result;
		result << msg << " (Failure with HRESULT of 0x" << std::hex << hr << ")";

		ThrowIfFailed(true, result.str(), loc);
	}
}

inline void ThrowIfFailed(HRESULT hr, const std::string& msg = "Unkown Error", const std::source_location& loc = std::source_location::current()) {
	ThrowIfFailed(hr, msg.c_str(), loc);
}