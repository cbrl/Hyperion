#pragma once

//----------------------------------------------------------------------------------
// Pointer datatypes
//----------------------------------------------------------------------------------

#if defined(_WIN32)

// Smart pointer for COM objects
#include "os/windows.h"
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#endif //defined(_WIN32)


#include <memory>

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

using std::make_unique;
using std::make_shared;

using intptr = std::intptr_t;
using uintptr = std::uintptr_t;
