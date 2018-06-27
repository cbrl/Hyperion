#pragma once

//----------------------------------------------------------------------------------
// Pointer datatypes
//----------------------------------------------------------------------------------
#include <memory>

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

using std::make_unique;
using std::make_shared;

using intptr = std::intptr_t;
using uintptr = std::uintptr_t;
