#pragma once

#include "directx/d3d11.h"
#include "io/io.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"


namespace ModelLoader {

	template<typename VertexT>
	[[nodiscard]]
	ModelOutput<VertexT> load(ResourceMgr& resource_mgr, const fs::path& file);
};


#include "model_loader.tpp"
