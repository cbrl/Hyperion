module;

#include "io/io.h"

export module rendering:importer.model_importer;

import log;

import :importer.assimp_importer;
import :model_output;
import :model_config;
import :resource_mgr;


export namespace render::importer {

template<typename VertexT>
[[nodiscard]]
ModelOutput ImportModel(ResourceMgr& resource_mgr, const fs::path& file, const ModelConfig<VertexT>& config) {

	Logger::log(LogLevel::info, "Loading model: {}", file.string());
	auto out = detail::AssimpImport(resource_mgr, file, config.flip_winding, config.flip_uv);
	Logger::log(LogLevel::info, "Loaded model: {}", file.string());
	return out;
}

} //namespace render::importer
