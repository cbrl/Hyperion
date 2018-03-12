#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "util\io\io.h"
#include "resource\model\model_blueprint.h"
#include "resource\resource_mgr.h"
#include "loader\obj_loader.h"


class ModelFactory {
	public:
		ModelFactory() {
			obj_loader = make_unique<OBJLoader>();
		}
		~ModelFactory() = default;

		Model CreateModel(ID3D11Device* device,
		                  ResourceMgr& resource_mgr,
		                  wstring folder, wstring filename,
		                  bool right_hand_coords) {

			// TODO: Check file extension, then use the appropriate loader

			// Create a model from an obj file
			return obj_loader->Load(device, resource_mgr, folder, filename, right_hand_coords);
		}


	private:
		unique_ptr<OBJLoader> obj_loader;
};