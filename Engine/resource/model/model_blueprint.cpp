#include "stdafx.h"
#include "model_blueprint.h"

#include "loader\model_loader.h"


ModelBlueprint::ModelBlueprint(ID3D11Device* device,
							   ResourceMgr& resource_mgr,
							   const wstring& folder,
							   const wstring& filename) {

	auto out = ModelLoader::Load<VertexPositionNormalTexture>(device, resource_mgr, folder, filename);
	
	name        = out.name;
	materials   = out.materials;

	size_t group_count = out.groups.size();

	// Create the mesh
	mesh = make_shared<Mesh>(device, out.vertices, out.indices);

	// Create the AABB for the model
	auto pair = MinMaxPoint(out.vertices);
	aabb = AABB(pair.first, pair.second);

	for (size_t i = 0; i < group_count; ++i) {
		ModelPart temp;

		temp.name = out.groups[i].name;
		temp.index_start = out.groups[i].index_start;
		temp.material_index = out.groups[i].material_index;

		// Index count
		if (i == group_count - 1)
			temp.index_count = static_cast<u32>(out.indices.size() - temp.index_start);
		else
			temp.index_count = out.groups[i+1].index_start - temp.index_start;


		// Create the AABB for the model part
		vector<VertexPositionNormalTexture> subvec;

		for (size_t j = temp.index_start; j < (temp.index_start + temp.index_count); ++j) {
			subvec.push_back(out.vertices[out.indices[j]]);
		}

		auto pair = MinMaxPoint(subvec);
		temp.aabb = AABB(pair.first, pair.second);

		model_parts.push_back(temp);
	}
}