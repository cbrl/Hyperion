#pragma once

// Silence codecvt deprecation warning
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <type_traits>
#include <typeindex>
#include <variant>
#include "scalar_types.h"
#include "pointer_types.h"
#include "container_types.h"
#include "vector_types.h"
#include "string/string.h"

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)

// Matrix/vector types and related functions
#include <DirectXMath.h>
using namespace DirectX;

#endif //defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)


//----------------------------------------------------------------------------------
// Type Traits
//----------------------------------------------------------------------------------

using std::enable_if_t;
using std::is_same_v;
using std::type_info;
using std::type_index;
