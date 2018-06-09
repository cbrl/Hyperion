#pragma once

#include <d3d11.h>
#include "util/engine_util.h"
#include "datatypes/datatypes.h"
#include "io/io.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"
#include "loader/obj/obj_loader.h"


namespace ModelLoader {
	template<typename VertexT>
	ModelOutput<VertexT> Load(ID3D11Device& device,
	                          ResourceMgr& resource_mgr,
	                          const wstring& filename);
};


#include "model_loader.tpp"
