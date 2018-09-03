#pragma once

#include "resource/texture/texture.h"

class ResourceMgr;


namespace TextureFactory {

	shared_ptr<Texture> CreateDefaultTexture(ResourceMgr& resource_mgr);

	shared_ptr<Texture> CreateColorTexture(ResourceMgr& resource_mgr, const vec4_f32& color);
}