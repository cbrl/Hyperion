#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Windows.h>
#include <WinUser.h>

#include <wrl/client.h>

#include "datatypes/types.h"

using Microsoft::WRL::ComPtr;
