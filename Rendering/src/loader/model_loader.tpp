#include "log/log.h"

namespace ModelLoader {

	template<typename VertexT>
	[[nodiscard]]
	static ModelOutput<VertexT> load(ResourceMgr& resource_mgr, const fs::path& file) {

		// TODO:
		// Check file extension, then use the appropriate loader. For
		// now, the only loader is the obj loader.


		// Create a model blueprint from an obj file
		ModelOutput<VertexT> out = OBJLoader<VertexT>::load(resource_mgr, file, false);
		Logger::log(LogLevel::info, "Loaded OBJ file: {}", file.string());

		return out;
	}
}
