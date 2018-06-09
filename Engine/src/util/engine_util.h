#pragma once

#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include "log/log.h"


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
	com_exception(HRESULT hr, const char* msg = "") : result(hr), msg(msg) {
	}

	const char* what() const override {
		std::ostringstream stream;

		stream << msg << " (Failure with HRESULT of " << std::hex << result << ")";

		return stream.str().c_str();
	}

private:
	HRESULT result;
	std::string msg;
};


// Throw
inline void ThrowIfFailed(HRESULT hr, const char* msg = "") {
	if (FAILED(hr)) {
		FILE_LOG(logERROR) << msg << " (Failure with HRESULT of " << std::hex << hr << ")";
		throw com_exception(hr, msg);
	}
}

inline void ThrowIfFailed(bool result, const char* msg = "") {
	if (!result) {
		FILE_LOG(logERROR) << msg;
		throw std::exception(msg);
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
