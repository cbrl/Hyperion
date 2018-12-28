#pragma once

#include "io/io.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"
#include "resource/model/model_config.h"
#include "loader/assimp_loader.h"


namespace ModelLoader {

	template<typename VertexT>
	[[nodiscard]]
	ModelOutput Load(ResourceMgr& resource_mgr, const fs::path& file, const ModelConfig<VertexT>& config) {

		Logger::log(LogLevel::info, "Loading model: {}", file.string());
		auto out = AssimpLoader::Load(resource_mgr, file, config.flip_winding, config.flip_uv);
	    Logger::log(LogLevel::info, "Loaded model: {}", file.string());
	    return out;
	}
}
