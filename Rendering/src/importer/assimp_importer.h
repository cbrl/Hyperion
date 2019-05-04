#pragma once

#include "io/io.h"
#include "resource/model/model_output.h"

namespace render {

class ResourceMgr;

namespace importer::detail {
[[nodiscard]]
ModelOutput AssimpImport(ResourceMgr& resource_mgr,
	                     const fs::path& file,
	                     bool flip_winding,
	                     bool flip_uv);
} //namespace importer::detail

} //namespace render
