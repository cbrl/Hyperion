#pragma once

#include "io/io.h"
#include "resource/model/model_output.h"

namespace render {

class ResourceMgr;

namespace AssimpLoader {
[[nodiscard]]
ModelOutput Load(ResourceMgr& resource_mgr,
	             const fs::path& file,
	             bool flip_winding,
	             bool flip_uv);
} //namespace AssimpLoader

} //namespace render
