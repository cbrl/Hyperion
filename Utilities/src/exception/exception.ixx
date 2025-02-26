module;

#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <source_location>

#ifdef _WIN32
#include <Windows.h>
#endif

export module exception;

import log;

export {

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

#ifdef _WIN32
inline void ThrowIfFailed(HRESULT hr, const char* msg = "Unkown Error", const std::source_location& loc = std::source_location::current()) {
	if (FAILED(hr)) {
		std::stringstream result;
		result << msg << " (Failure with HRESULT of 0x" << std::hex << hr << ")";

		ThrowIfFailed(true, result.str(), loc);
	}
}

inline void ThrowIfFailed(HRESULT hr, const std::string& msg = "Unkown Error", const std::source_location& loc = std::source_location::current()) {
	ThrowIfFailed(hr, msg.c_str(), loc);
}
#endif //_WIN32

} //export