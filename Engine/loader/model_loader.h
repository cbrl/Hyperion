#pragma once

#include <d3d11.h>
#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "util\io\io.h"
#include "resource\resource_mgr.h"
#include "resource\model\model_output.h"
#include "loader\obj\obj_loader.h"


class ModelLoader {
	public:
		ModelLoader() = delete;

		template<typename VertexT>
		static ModelOutput<VertexT> Load(ID3D11Device* device,
										 ResourceMgr& resource_mgr,
										 const wstring& folder,
										 const wstring& filename) {

			// TODO:
			// Check file extension, then use the appropriate loader. For
			// now, the only loader is the obj loader.

			// Create a model blueprint from an obj file
			ModelOutput<VertexT> out = OBJLoader<VertexT>::Load(device, resource_mgr, folder, filename, false);
			FILE_LOG(logDEBUG) << "Loaded OBJ file: " << wstr2str(folder + filename);

			return out;
		}
};