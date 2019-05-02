#pragma once

#include "resource/model/material/material.h"

namespace render {

class ResourceMgr;

namespace MaterialFactory {
Material CreateDefaultMaterial(ResourceMgr& resource_mgr);
}

} //namespace render