#pragma once


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include <cassert>
#include <stdexcept>
#include <iostream>



//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

template<typename ExceptionT = std::runtime_error>
inline void ThrowIfFailed(bool result, const std::string& msg = "") {
	if (!result) {
		std::cerr << msg.c_str() << std::endl;
		throw ExceptionT(msg);
	}
}
