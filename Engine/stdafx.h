// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <wrl\client.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <memory>
#include <exception>
#include <vector>
#include <string>


// DirectX Header Files
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <DirectXColors.h>


// DirectX Libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


// User Created Files
#include "EngineUtil.h"