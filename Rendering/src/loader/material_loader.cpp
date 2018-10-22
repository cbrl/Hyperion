#include "material_loader.h"
#include "resource/resource_mgr.h"
#include "loader/mtl/mtl_loader.h"


namespace MaterialLoader {
	
	void load(ResourceMgr& resource_mgr, const fs::path& file, std::vector<Material>& materials) {
		
		if (file.extension().string() == ".mtl") {
			MtlLoader loader(resource_mgr, materials);
			loader.load(file);
			return;
		}
	}
}
