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

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Windows.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "exception/exception.h"
#include "datatypes/scalar_types.h"
#include "string/string.h"




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




//----------------------------------------------------------------------------------
// Error handling
//----------------------------------------------------------------------------------

// Throw
inline void ThrowIfFailed(HRESULT hr, const char* msg = "") {
	if (FAILED(hr)) {
		std::cerr << msg << " (Failure with HRESULT of 0x" << std::hex << hr << std::dec << ")\n";
		throw Exception(msg);
	}
}


// Alert Window
inline void AlertIfFailed(HRESULT hr, const wchar_t* msg = L"") {
	if (FAILED(hr)) {
		AlertIfFailed(false, msg);
	}
}

inline void AlertIfFailed(bool result, const wchar_t* msg = L"") {
	if (!result) {
		if (wcsncmp(msg, L"", 1) == 0) {
			MessageBox(nullptr, msg, L"Error", MB_OK);
		}
		else {
			LPWSTR output;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
						  FORMAT_MESSAGE_IGNORE_INSERTS |
						  FORMAT_MESSAGE_ALLOCATE_BUFFER,
						  nullptr,
						  E_FAIL,
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  reinterpret_cast<LPTSTR>(&output),
						  0,
						  nullptr);
			MessageBox(nullptr, output, L"Error", MB_OK);
		}
	}
}
