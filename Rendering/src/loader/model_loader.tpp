#include "log/log.h"

namespace ModelLoader {

	template<typename VertexT>
	[[nodiscard]]
	static ModelOutput<VertexT> load(ResourceMgr& resource_mgr, const std::wstring& filename) {

		// TODO:
		// Check file extension, then use the appropriate loader. For
		// now, the only loader is the obj loader.


		// Create a model blueprint from an obj file
		ModelOutput<VertexT> out = OBJLoader<VertexT>::load(resource_mgr, filename, false);
		Logger::log(LogLevel::info, "Loaded OBJ file: {}", wstr2str(filename));

		return out;
	}
}
