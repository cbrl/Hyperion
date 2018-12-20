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
#include <gsl/pointers>

