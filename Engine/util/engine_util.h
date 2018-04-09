#pragma once

#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include "util\log\log.h"


#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


// Include SetDebugObjectName()
#include <DirectXHelpers.h>
using DirectX::SetDebugObjectName;


//----------------------------------------------------------------------------------
// Error handling
//----------------------------------------------------------------------------------

// Helper class for COM exceptions
class com_exception : public std::exception {
	public:
		com_exception(HRESULT hr, std::string msg = "") : result(hr), msg(msg) {}

		virtual const char* what() const override {
			std::ostringstream stream;

			stream << msg << " (Failure with HRESULT of " << std::hex << result << ")";

			return stream.str().c_str();
		}

	private:
		HRESULT result;
		std::string msg;
};


// Throw
inline void ThrowIfFailed(HRESULT hr, std::string msg = "") {
	if (FAILED(hr)) {
		FILE_LOG(logERROR) << msg << " (Failure with HRESULT of " << std::hex << hr << ")";
		throw com_exception(hr, msg);
	}
}

inline void ThrowIfFailed(bool result, std::string msg = "") {
	if (!result) {
		FILE_LOG(logERROR) << msg;
		throw std::exception(msg.c_str());
	}
}


// Log
inline void LogIfFailed(HRESULT hr, const char* msg = "", TLogLevel log_level = logWARNING) {
	if (FAILED(hr)) {
		LogIfFailed(false, msg, log_level);
	}
}

inline void LogIfFailed(bool result, const char* msg = "", TLogLevel log_level = logWARNING) {
	if (!result) {
		FILE_LOG(log_level) << msg;
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
		if (msg != L"") {
			MessageBox(NULL, msg, L"Error", MB_OK);
		}
		else {
			LPWSTR output;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
							FORMAT_MESSAGE_IGNORE_INSERTS |
							FORMAT_MESSAGE_ALLOCATE_BUFFER,
							NULL,
							E_FAIL,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR)&output,
							0,
							NULL);
			MessageBox(NULL, output, L"Error", MB_OK);
		}
	}
}