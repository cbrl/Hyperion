#include "log/log.h"
#include "loader/obj/obj_loader.h"


namespace ModelLoader {

	template<typename VertexT>
	static ModelOutput<VertexT> load(ResourceMgr& resource_mgr, const fs::path& file) {

		// TODO:
		// Check file extension, then use the appropriate loader. For
		// now, the only loader is the obj loader.


		ObjLoader<VertexT> loader(resource_mgr);
		auto out = loader.load(file, false);

		Logger::log(LogLevel::info, "Loaded OBJ file: {}", file.string());
		return out;
	}
}
