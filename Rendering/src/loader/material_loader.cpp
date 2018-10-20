#include "material_loader.h"
#include "resource/resource_mgr.h"
#include "loader/mtl/mtl_loader.h"


namespace MaterialLoader {
	
	void load(const fs::path& file, ResourceMgr& resource_mgr, std::vector<Material>& materials) {
		
		if (file.extension().string() == ".mtl") {
			MtlLoader loader(resource_mgr, materials);
			loader.load(file);
			return;
		}
	}
}
