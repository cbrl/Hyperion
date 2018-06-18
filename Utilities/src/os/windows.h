#pragma once


//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

// Exclude rarely used stuff from windows headers
#define WIN32_LEAN_AND_MEAN

// Don't define the windows min/max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Silence warning for codecvt deprecation
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif



//----------------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------------

#include <Windows.h>
#include "datatypes/scalar_types.h"



//----------------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------------

// Convert a time represetned by a FILETIME struct
// to a single 64 bit integer (units: 100ns)
inline u64 FiletimeToU64(const FILETIME& time) {
	return static_cast<u64>(time.dwLowDateTime)
	       | (static_cast<u64>(time.dwHighDateTime) << 32);
}


// Outputs the amount of time the process has
// executed in kernel and user modes (units: 100ns)
inline void GetProcessTimes(u64& kernel_time, u64& user_time) {
	FILETIME ftime;
	FILETIME fkernel;
	FILETIME fuser;

	const BOOL result = GetProcessTimes(GetCurrentProcess(), &ftime, &ftime, &fkernel, &fuser);

	if (result == TRUE) {
		kernel_time = FiletimeToU64(fkernel);
		user_time   = FiletimeToU64(fuser);
	}
	else {
		kernel_time = 0;
		user_time   = 0;
	}
}


// Outputs the amount of time the system has
// executed in kernel and user modes (units: 100ns)
inline void GetSystemWorkTimes(u64& kernel_time, u64& user_time) {
	FILETIME fidle;
	FILETIME fkernel;
	FILETIME fuser;

	const BOOL result = GetSystemTimes(&fidle, &fkernel, &fuser);

	if (result == TRUE) {
		kernel_time = FiletimeToU64(fkernel);
		user_time   = FiletimeToU64(fuser);
	}
	else {
		kernel_time = 0;
		user_time   = 0;
	}
}


// Returns the total time the system has been idle
inline u64 GetSystemIdleTime() {
	FILETIME fidle;
	FILETIME fkernel;
	FILETIME fuser;

	const BOOL result = GetSystemTimes(&fidle, &fkernel, &fuser);

	return result == TRUE ? FiletimeToU64(fidle) : 0;
}


// Returns the total time the system has executed (kernel + user, units: 100ns)
inline u64 GetSystemWorkTime() {
	u64 kernel_time = 0;
	u64 user_time = 0;
	GetSystemWorkTimes(kernel_time, user_time);
	return kernel_time + user_time;
}


// Returns the total time the process has executed (kernel + user, units: 100ns)
inline u64 GetCPUTime() {
	u64 kernel_time = 0;
	u64 user_time = 0;
	GetProcessTimes(kernel_time, user_time);
	return kernel_time + user_time;
}
