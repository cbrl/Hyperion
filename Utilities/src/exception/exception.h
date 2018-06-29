#pragma once

#include <cassert>
#include <exception>
#include <iostream>


// Throw and log if the condition fails
inline void ThrowIfFailed(bool result, const char* msg = "") {
	if (!result) {
		std::cerr << msg << std::endl;
		throw std::exception(msg);
	}
}