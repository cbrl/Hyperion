#include "material_factory.h"
#include "resource/resource_mgr.h"
#include "resource/texture/texture_factory.h"


namespace render::MaterialFactory {

Material CreateDefaultMaterial(ResourceMgr& resource_mgr) {
	return Material{
		.name                 = "Default Material",
		.params{
			.base_color    = {1.0f, 1.0f, 1.0f, 1.0f},
			.metalness     = 0.9f,
			.roughness     = 0.7f,
			.emissive      = 0.0f,
		},
		.maps{
			.base_color      = TextureFactory::CreateWhiteTexture(resource_mgr),
			.material_params = TextureFactory::CreateWhiteTexture(resource_mgr),
			.normal          = TextureFactory::CreateBlackTexture(resource_mgr),
			.emissive        = TextureFactory::CreateBlackTexture(resource_mgr),
		}
	};
}

} //namespace render::MaterialFactory
