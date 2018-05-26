#include "stdafx.h"
#include "model_blueprint.h"

#include "loader\model_loader.h"


ModelBlueprint::ModelBlueprint(ID3D11Device& device,
							   ResourceMgr& resource_mgr,
							   const wstring& filename) {

	auto out = ModelLoader::Load<VertexPositionNormalTexture>(device, resource_mgr, filename);
	
	ConstructBlueprint(device, out);
}
