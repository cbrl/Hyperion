#pragma once

#include "resource/texture/texture.h"

class ResourceMgr;


namespace TextureFactory {

	// Create a default checkerboard texture
	std::shared_ptr<Texture> CreateDefaultTexture(ResourceMgr& resource_mgr);

	// Create a 1x1 texture from a specified color (RGBA)
	std::shared_ptr<Texture> CreateColorTexture(ResourceMgr& resource_mgr, const vec4_f32& color);

	// Create a white texture
	std::shared_ptr<Texture> CreateWhiteTexture(ResourceMgr& resource_mgr);

	// Create a black texture
	std::shared_ptr<Texture> CreateBlackTexture(ResourceMgr& resource_mgr);

	// Create a red texture
	std::shared_ptr<Texture> CreateRedTexture(ResourceMgr& resource_mgr);

	// Create a green texture
	std::shared_ptr<Texture> CreateGreenTexture(ResourceMgr& resource_mgr);

	// Create a blue texture
	std::shared_ptr<Texture> CreateBlueTexture(ResourceMgr& resource_mgr);

	// Create a cyan texture
	std::shared_ptr<Texture> CreateCyanTexture(ResourceMgr& resource_mgr);

	// Create a magenta texture
	std::shared_ptr<Texture> CreateMagentaTexture(ResourceMgr& resource_mgr);

	// Create a yello texture
	std::shared_ptr<Texture> CreateYellowTexture(ResourceMgr& resource_mgr);

}