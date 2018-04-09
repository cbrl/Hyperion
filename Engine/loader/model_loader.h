#pragma once

#include <d3d11.h>
#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "util\io\io.h"
#include "resource\resource_mgr.h"
#include "resource\model\model_output.h"
#include "loader\obj\obj_loader.h"


class ModelLoader final {
	public:
		ModelLoader() = delete;

		template<typename VertexT>
		static ModelOutput<VertexT> Load(ID3D11Device* device,
										 ResourceMgr& resource_mgr,
										 const wstring& filename);
};


#include "model_loader.tpp"