#pragma once

#include <exception>
#include <sstream>
#include <string>

using std::string;
using std::wstring;
using std::ostringstream;

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


//----------------------------------------------------------------------------------
// D3D error checker
//----------------------------------------------------------------------------------

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                          \
	{                                                      \
		HRESULT hr = (x);                                  \
		if(FAILED(hr))                                     \
		{                                                  \
			LPWSTR output;                                 \
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |     \
				FORMAT_MESSAGE_IGNORE_INSERTS        |     \
				FORMAT_MESSAGE_ALLOCATE_BUFFER,            \
				NULL,                                      \
				hr,                                        \
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
				(LPTSTR) &output,                          \
				0,                                         \
				NULL);                                     \
			MessageBox(NULL, output, L"Error", MB_OK);     \
		}                                                  \
	}
	#endif
#else
	#ifndef HR
	#define HR(x)                   \
	{                               \
		HRESULT hr = (x);           \
		if(FAILED(hr))              \
		{                           \
			throw std::exception(); \
		}                           \
	}
	#endif
#endif

namespace DX {
	// Helper class for COM exceptions
	class com_exception : public std::exception {
		public:
			com_exception(HRESULT hr, string msg = "") : result(hr), msg(msg) {}

			virtual const char* what() const override {
				ostringstream stream;

				stream << msg << " (Failure with HRESULT of " << static_cast<unsigned int>(result);

				return stream.str().c_str();
			}

		private:
			HRESULT result;
			string msg;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr, string msg = "") {
		if (FAILED(hr)) {
			throw com_exception(hr, msg);
		}
	}

	inline void AlertIfFailed(HRESULT hr, wstring msg = L"") {
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


//----------------------------------------------------------------------------------
// Helper for releasing COM objects
//----------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }


//----------------------------------------------------------------------------------
// Helper for deleting objects
//----------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = nullptr; }


//----------------------------------------------------------------------------------
// Shader Type enum
//----------------------------------------------------------------------------------

enum class ShaderTypes {
	ColorShader,
	TextureShader,
	LightShader,
	NormalShader,
	SpecularShader,
};


//----------------------------------------------------------------------------------
// Buffer Type enum
//----------------------------------------------------------------------------------

enum class BufferTypes {
	MatrixBuffer,
	CameraBuffer,
	LightBuffer
};