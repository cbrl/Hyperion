#pragma once

#include "io/io.h"
#include "resource/model/material/material.h"

class ResourceMgr;

namespace MaterialLoader {
	
	void load(ResourceMgr& resource_mgr, const fs::path& file, std::vector<Material>& materials);
}
