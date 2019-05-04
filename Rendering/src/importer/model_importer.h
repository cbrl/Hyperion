#pragma once

#include "io/io.h"
#include "resource/resource_mgr.h"
#include "resource/model/model_output.h"
#include "resource/model/model_config.h"
#include "importer/assimp_importer.h"


namespace render::importer {

template<typename VertexT>
[[nodiscard]]
ModelOutput ImportModel(ResourceMgr& resource_mgr, const fs::path& file, const ModelConfig<VertexT>& config) {

	Logger::log(LogLevel::info, "Loading model: {}", file.string());
	auto out = detail::AssimpImport(resource_mgr, file, config.flip_winding, config.flip_uv);
	Logger::log(LogLevel::info, "Loaded model: {}", file.string());
	return out;
}

} //namespace render::importer
