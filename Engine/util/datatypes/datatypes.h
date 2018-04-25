#include <cstdint>
#include <type_traits>
#include <typeindex>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <variant>
#include <wrl\client.h>

#include "util\string\string.h"
#include "util\datatypes\vector_types.h"
#include "util\datatypes\vertex_types.h"


//----------------------------------------------------------------------------------
// Type Info
//----------------------------------------------------------------------------------

using std::enable_if_t;
using std::is_same_v;
using std::type_info;
using std::type_index;


//----------------------------------------------------------------------------------
// Pointer Types
//----------------------------------------------------------------------------------

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using Microsoft::WRL::ComPtr;

using std::make_unique;
using std::make_shared;

using intptr  = std::intptr_t;
using uintptr = std::uintptr_t;


//----------------------------------------------------------------------------------
// Vector Types
//----------------------------------------------------------------------------------

using std::vector;
using std::map;
using std::unordered_map;


//----------------------------------------------------------------------------------
// Shorthand for fixed-size datatypes
//----------------------------------------------------------------------------------

// Signed int
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// Unsigned	int
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
