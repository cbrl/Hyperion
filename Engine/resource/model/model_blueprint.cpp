#include "stdafx.h"
#include "model_blueprint.h"
#include "loader\model_loader.h"


ModelBlueprint::ModelBlueprint(ID3D11Device* device,
							   ResourceMgr& resource_mgr,
							   const wstring& folder,
							   const wstring& filename) {

	ModelLoader::Load(device, resource_mgr, folder, filename, *this);
}