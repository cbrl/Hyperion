module;

#include "datatypes/vector_types.h"

export module rendering.material_factory;

export import rendering.material;
import rendering.texture_factory;
import rendering.resource_mgr;

namespace render {

export namespace MaterialFactory {
Material CreateDefaultMaterial(ResourceMgr& resource_mgr) {
	return Material{
		.name = "Default Material",
		.params{
			.base_color = {1.0f, 1.0f, 1.0f, 1.0f},
			.metalness  = 0.9f,
			.roughness  = 0.7f,
			.emissive   = 0.0f,
		},
		.maps{
			.base_color      = TextureFactory::CreateWhiteTexture(resource_mgr),
			.material_params = TextureFactory::CreateWhiteTexture(resource_mgr),
			.normal          = TextureFactory::CreateBlackTexture(resource_mgr),
			.emissive        = TextureFactory::CreateBlackTexture(resource_mgr),
		}
	};
}
} //namespace MaterialFactory

} //namespace render
