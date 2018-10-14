#pragma once


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include "string/string.h"
#include <cassert>
#include <stdexcept>
#include <iostream>



//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

inline void ThrowIfFailed(bool result, const std::string& msg = "") {
	if (!result) {
		//std::cerr << msg << std::endl;
		throw std::runtime_error(msg);
	}
}
