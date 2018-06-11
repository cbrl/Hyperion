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

#include <Windows.h>