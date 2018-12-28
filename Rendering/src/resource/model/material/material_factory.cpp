#include "material_factory.h"
#include "resource/resource_mgr.h"
#include "resource/texture/texture_factory.h"


namespace MaterialFactory {

Material CreateDefaultMaterial(ResourceMgr& resource_mgr) {
	Material mat;

	mat.name                 = "Default Material";
	mat.params.base_color    = {1.0f, 1.0f, 1.0f, 1.0f};
	mat.params.metalness     = 0.0f;
	mat.params.roughness     = 1.0f;
	mat.params.emissive      = 0.0f;
	mat.maps.base_color      = TextureFactory::CreateWhiteTexture(resource_mgr);
	mat.maps.material_params = TextureFactory::CreateWhiteTexture(resource_mgr);
	mat.maps.normal          = TextureFactory::CreateBlackTexture(resource_mgr);
	mat.maps.emissive        = TextureFactory::CreateBlackTexture(resource_mgr);

	return mat;
}

}
