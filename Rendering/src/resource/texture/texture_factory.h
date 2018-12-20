#pragma once

#include "resource/texture/texture.h"

class ResourceMgr;


namespace TextureFactory {

std::shared_ptr<Texture> CreateDefaultTexture(ResourceMgr& resource_mgr);

std::shared_ptr<Texture> CreateColorTexture(ResourceMgr& resource_mgr, const vec4_f32& color);
}