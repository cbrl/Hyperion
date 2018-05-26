#pragma once

#include "datatypes\datatypes.h"


namespace AllocatorUtil {

	// Calculate the padding needed to align an address
	static inline const size_t CalculatePadding(const uintptr address, const size_t alignment) {

		size_t pad =  alignment - (address & static_cast<uintptr>(alignment - 1));

		return (pad == alignment) ? 0 : pad;
	}


	static inline const size_t CalculatePadding(const uintptr address,
												const size_t  alignment,
												const size_t  header_size) {

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