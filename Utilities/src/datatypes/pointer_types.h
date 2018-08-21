#pragma once

//----------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------

// Fix MSVC error with std::aligned_storage possible when creating a std::shared_ptr
#undef _ENABLE_EXTENDED_ALIGNED_STORAGE
#define _ENABLE_EXTENDED_ALIGNED_STORAGE


//----------------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------------

#include <memory>


//----------------------------------------------------------------------------------
// Aliases and "using" declarations
//----------------------------------------------------------------------------------

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

using intptr  = intptr_t;
using uintptr = uintptr_t;
