#pragma once

#include <exception>

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


//----------------------------------------------------------------------------------
// Helper for releasing COM objects
//----------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }


//----------------------------------------------------------------------------------
// Helper for deleting objects
//----------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = nullptr; }


//----------------------------------------------------------------------------------
// Shader Type enum for selecting which shader to use
//----------------------------------------------------------------------------------

enum class ShaderTypes {
	ColorShader    = 0,
	TextureShader  = 1,
	LightShader    = 2,
	NormalShader   = 3,
	SpecularShader = 4,
};
