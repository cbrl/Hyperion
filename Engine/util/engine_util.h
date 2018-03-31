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
		FILE_LOG(logERROR) << msg;
		throw com_exception(hr, msg);
	}
}

inline void ThrowIfFailed(bool result, std::string msg = "") {
	if (!result) {
		FILE_LOG(logERROR) << msg;
		throw std::exception(msg.c_str());
	}
}


// Alert Window
inline void AlertIfFailed(HRESULT hr, std::wstring msg = L"") {
	if (FAILED(hr)) {
		if (!msg.empty()) {
			MessageBox(NULL, msg.c_str(), L"Error", MB_OK);
		}
		else {
			LPWSTR output;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
							FORMAT_MESSAGE_IGNORE_INSERTS |
							FORMAT_MESSAGE_ALLOCATE_BUFFER,
							NULL,
							hr,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR)&output,
							0,
							NULL);
			MessageBox(NULL, output, L"Error", MB_OK);
		}
	}
}

inline void AlertIfFailed(bool result, std::wstring msg = L"") {
	if (!result) {
		if (!msg.empty()) {
			MessageBox(NULL, msg.c_str(), L"Error", MB_OK);
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