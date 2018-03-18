#include <cstdint>
#include <vector>
#include <map>
#include <memory>
#include <type_traits>
#include <wrl\client.h>

#include "util\string\string.h"
#include "util\datatypes\vector_types.h"
#include "util\datatypes\vertex_types.h"


using std::enable_if_t;
using std::is_same_v;


//----------------------------------------------------------------------------------
// Pointer Types
//----------------------------------------------------------------------------------

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using Microsoft::WRL::ComPtr;

using std::make_unique;
using std::make_shared;


//----------------------------------------------------------------------------------
// Vector Types
//----------------------------------------------------------------------------------

using std::vector;
using std::map;


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