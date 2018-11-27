#pragma once

#include "io/io.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"


namespace ModelLoader {

	[[nodiscard]]
	ModelOutput load(ResourceMgr& resource_mgr, const fs::path& file);
}
