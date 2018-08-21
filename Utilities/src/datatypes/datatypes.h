#pragma once

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

// Silence codecvt deprecation warning
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif

// Declare std::aligned_storage behavior for MSVC
#undef _ENABLE_EXTENDED_ALIGNED_STORAGE
#define _ENABLE_EXTENDED_ALIGNED_STORAGE


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------


#include "scalar_types.h"
#include "pointer_types.h"
#include "container_types.h"
#include "vector_types.h"
#include "string/string.h"
#include <typeindex>
