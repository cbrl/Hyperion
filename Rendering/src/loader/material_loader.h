#pragma once

#include "io/io.h"
#include "resource/model/material/material.h"

class ResourceMgr;

namespace MaterialLoader {
	
	void load(const fs::path& file, ResourceMgr& resource_mgr, std::vector<Material>& materials);
}
