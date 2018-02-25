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
#include <timeapi.h>


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <math.h>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <memory>
#include <exception>
#include <vector>
#include <string>
#include <map>
#include <type_traits>
#include <algorithm>


// DirectX Header Files
#include <d3d11.h>
#include <dxgi.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <DirectXHelpers.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <DirectXColors.h>


// DirectX Libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


// User Created Files
#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "util\\io\io.h"
#include "util\math\math.h"
#include "util\string\string.h"