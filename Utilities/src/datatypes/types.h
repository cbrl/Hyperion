#pragma once

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

// Silence codecvt deprecation warning
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif

// Declare std::aligned_storage behavior for MSVC
#ifndef _ENABLE_EXTENDED_ALIGNED_STORAGE
#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#endif


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------


#include "scalar_types.h"
#include "pointer_types.h"
#include "container_types.h"
#include "vector_types.h"
#include "string/string.h"

// Type information and properties
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <concepts>

// Microsoft GSL
#include <gsl/gsl>