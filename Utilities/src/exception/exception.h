#pragma once

#include <cassert>
#include <exception>
#include "log/log.h"


// Throw and log if the condition fails
inline void ThrowIfFailed(bool result, const char* msg = "") {
	if (!result) {
		FILE_LOG(logERROR) << msg;
		throw std::exception(msg);
	}
}