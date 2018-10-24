#pragma once


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include "string/string.h"
#include "log/log.h"
#include <cassert>
#include <stdexcept>
#include <iostream>



//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

#define EXPAND(x) x
#define GET_THROW_MACRO(_2, _1, NAME, ...) NAME
#define ThrowIfFailed_1(result) ThrowIfFailed(result, __FILE__, __LINE__)
#define ThrowIfFailed_2(result, msg) ThrowIfFailed(result, __FILE__, __LINE__, msg)
#define ThrowIfFailed(...) EXPAND(GET_THROW_MACRO(__VA_ARGS__, ThrowIfFailed_2, ThrowIfFailed_1)(__VA_ARGS__))

inline void (ThrowIfFailed)(bool result, const char* file, int line, const std::string& msg = "Unknown Error") {
	if (!result) {
		//std::cerr << msg << std::endl;
		Logger::log(LogLevel::critical, "{}:{:d} - {}", file, line, msg);
		throw std::runtime_error(msg);
	}
}
