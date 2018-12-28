#pragma once

#include "resource/model/material/material.h"

class ResourceMgr;

namespace MaterialFactory {

	Material CreateDefaultMaterial(ResourceMgr& resource_mgr);
}