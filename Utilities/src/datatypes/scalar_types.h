#pragma once


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include <cstdint>


//----------------------------------------------------------------------------------
// Aliases
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

// Floating point
using f32 = float;
using f64 = double;
static_assert(sizeof(f32) == 4, "Float size mismatch");
static_assert(sizeof(f64) == 8, "Double size mismatch");