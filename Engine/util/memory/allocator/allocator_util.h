#pragma once

#include "util\datatypes\datatypes.h"


namespace AllocatorUtil {

	static inline const size_t CalculatePadding(const uptr address, const size_t alignment) {

		return alignment - (address & static_cast<uptr>(alignment - 1));
	}


	static inline const size_t CalculatePadding(const uptr   address,
												const size_t alignment,
												const size_t header_size) {

		size_t padding = CalculatePadding(address, alignment);

		size_t space_needed = header_size;

		// Increase padding to fit header if it's too small
		if (padding < space_needed) {
			space_needed -= padding;

			if (space_needed % alignment > 0) {
				padding += alignment * (1 + (space_needed / alignment));
			}
			else {
				padding += alignment * (space_needed / alignment);
			}
		}

		return padding;
	}
}