#pragma once

#include "datatypes/scalar_types.h"
#include "datatypes/vector_types.h"
#include <cmath>
#include <limits>


// Convert an rgba vec4 [0.0f, 1.0f] to a hex color value
[[nodiscard]]
inline u32 Float4ColorToU32(const vec4_f32& color) {
	return static_cast<u32>(color.x * 0xff)            //R
	       | (static_cast<u32>(color.y * 0xff) << 8)   //G
	       | (static_cast<u32>(color.z * 0xff) << 16)  //B
	       | (static_cast<u32>(color.w * 0xff) << 24); //A
}