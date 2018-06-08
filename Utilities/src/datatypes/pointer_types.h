#pragma once

//----------------------------------------------------------------------------------
// Pointer datatypes
//----------------------------------------------------------------------------------

#include <memory>

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)

// Smart pointer for COM objects
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#endif //defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_)


using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

using std::make_unique;
using std::make_shared;

using intptr = std::intptr_t;
using uintptr = std::uintptr_t;
