#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "io/io.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"
#include "loader/obj/obj_loader.h"


namespace ModelLoader {
	template<typename VertexT>
	[[nodiscard]]
	ModelOutput<VertexT> load(ResourceMgr& resource_mgr, const std::wstring& filename);
};


#include "model_loader.tpp"
