#include "model_blueprint.h"
#include "loader/model_loader.h"


ModelBlueprint::ModelBlueprint(ID3D11Device& device,
                               ResourceMgr& resource_mgr,
                               const std::wstring& filename)
	: Resource(filename) {

	const auto out = ModelLoader::load(resource_mgr, filename);
	constructBlueprint<VertexPositionNormalTexture>(device, out);
}