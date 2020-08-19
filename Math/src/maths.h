#pragma once

#include "datatypes/types.h"
#include <cmath>
#include <numeric>
#include <numbers>
#include <ratio>
#include <complex>
#include <valarray>
#include <limits>


// Convert an rgba vec4 [0.0f, 1.0f] to a hex color value
[[nodiscard]]
inline constexpr u32 Float4ColorToU32(const f32_4& color) noexcept {
	return static_cast<u32>(color[0] * 0xff)            //R
	       | (static_cast<u32>(color[1] * 0xff) << 8)   //G
	       | (static_cast<u32>(color[2] * 0xff) << 16)  //B
	       | (static_cast<u32>(color[3] * 0xff) << 24); //A
}


// Find the minimum and maximum points among a set of points
template<typename T, size_t N>
[[nodiscard]]
inline std::pair<Vector<T, N>, Vector<T, N>> MinMaxPoint(const std::vector<Vector<T, N>>& points) noexcept {
	Vector<T, N> min{std::numeric_limits<T>::max()};
	Vector<T, N> max{std::numeric_limits<T>::lowest()};

	for (const f32_3& point : points) {
		for (size_t i = 0; i < point.size(); ++i) {
			min[i] = std::fmin(min[i], point[i]);
			max[i] = std::fmax(max[i], point[i]);
		}
	}

	return {min, max};
}