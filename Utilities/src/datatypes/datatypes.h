// Silence codecvt deprecation warning
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1
#endif

#include <cstdint>
#include <type_traits>
#include <typeindex>
#include <variant>
#include "pointer_types.h"
#include "container_types.h"
#include "vector_types.h"
#include "string\string.h"

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


//----------------------------------------------------------------------------------
// Shorthand for fixed-size datatypes
//----------------------------------------------------------------------------------

// Signed int
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// Unsigned	int
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
