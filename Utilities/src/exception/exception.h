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
// Defines
//----------------------------------------------------------------------------------

// Utility macros
#define EXPAND(...) __VA_ARGS__
#define CAT(a, ...) a ## __VA_ARGS__

// Macro selection based on argument count
#define SELECT(name, num) CAT(name##_, num)
#define GET_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, count, ...) count
#define VA_SIZE(...) EXPAND(GET_COUNT(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1))
#define VA_SELECT(name, ...) SELECT(name, VA_SIZE(__VA_ARGS__))(__VA_ARGS__)

// ThrowIfFailed macros
#define ThrowIfFailed_1(result) ThrowIfFailed(result, __FILE__, __LINE__)
#define ThrowIfFailed_2(result, msg) ThrowIfFailed(result, __FILE__, __LINE__, msg)

#define ThrowIfFailed(...) VA_SELECT(ThrowIfFailed, __VA_ARGS__)


//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

inline void (ThrowIfFailed)(bool result, const char* file, int line, const std::string& msg = "Unknown Error") {
	if (!result) {
		//std::cerr << msg << std::endl;
		Logger::log(LogLevel::critical, "{}:{:d} - {}", file, line, msg);
		throw std::runtime_error(msg);
	}
}
