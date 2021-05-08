#pragma once


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include "log/log.h"
#include <string>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <source_location>


//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

inline void ThrowIfFailed(bool result, const char* msg = "Unknown Error", const std::source_location& loc = std::source_location::current()) {
	if (!result) {
		//std::cerr << msg << std::endl;
		Logger::log(LogLevel::critical, "{}:{:d} -- {} -- {}", loc.file_name(), loc.line(), loc.function_name(), msg);
		throw std::runtime_error(msg);
	}
}

inline void ThrowIfFailed(bool result, const std::string& msg = "Unknown Error", const std::source_location& loc = std::source_location::current()) {
	ThrowIfFailed(result, msg.c_str(), loc);
}
