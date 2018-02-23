#pragma once

#include <exception>
#include <fstream>
#include <sstream>
#include <string>


#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


// Include for SetDebugObjectName()
#include <DirectXHelpers.h>
using DirectX::SetDebugObjectName;


//----------------------------------------------------------------------------------
// Error handler
//----------------------------------------------------------------------------------

namespace DX {
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

	// Helper utility converts D3D API failures into exceptions
	inline void ThrowIfFailed(HRESULT hr, std::string msg = "") {
		if (FAILED(hr)) {
			throw com_exception(hr, msg);
		}
	}

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

	inline void LogIfFailed(HRESULT hr, std::string msg = "") {
		if (FAILED(hr)) {
			std::ofstream file;

			file.open("log.txt", std::ofstream::out | std::ofstream::app);
			if (file.fail()) {
				return;
			}

			file << "\n" << msg << " (Failure with HRESULT of " << std::hex << hr << ")";
			file.close();
		}
	}
}


//----------------------------------------------------------------------------------
// Helper for releasing COM objects
//----------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }


//----------------------------------------------------------------------------------
// Helper for deleting objects
//----------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = nullptr; }


