#include "stdafx.h"
#include "model_blueprint.h"

#include "loader/model_loader.h"


ModelBlueprint::ModelBlueprint(ID3D11Device& device,
                               ResourceMgr& resource_mgr,
                               const wstring& filename) {

	const auto out = ModelLoader::load<VertexPositionNormalTexture>(resource_mgr, filename);

	constructBlueprint(device, out);
}
