#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "util\io\io.h"
#include "resource\resource_mgr.h"
#include "resource\model\model_blueprint.h"
#include "loader\obj\obj_loader.h"


class ModelLoader {
	public:
		ModelLoader() = delete;

		static void Load(ID3D11Device* device,
						 ResourceMgr& resource_mgr,
						 const wstring& folder,
						 const wstring& filename,
						 ModelBlueprint& blueprint_out) {

			// TODO:
			// Check file extension, then use the appropriate loader. For
			// now, the only loader is the obj loader.

			// Create a model blueprint from an obj file
			OBJLoader::Load(device, resource_mgr, folder, filename, false, blueprint_out);
		}
};