#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "util\math\math.h"
#include "resource\resource_mgr.h"
#include "resource\mesh\mesh.h"
#include "resource\material\material.h"
#include "geometry\boundingvolume\bounding_volume.h"


class ResourceMgr;


struct ModelPart {
	wstring name;
	u32  index_start;
	u32  index_count;
	u32  material_index;
	AABB aabb;
};


struct Group {
	wstring name;
	u32 index_start;
	u32 material_index;
};


class ModelBlueprint {
	public:
		ModelBlueprint() = default;

		ModelBlueprint(ID3D11Device* device,
					   ResourceMgr& resource_mgr,
					   const wstring& folder,
					   const wstring& filename);

		template<typename VertexT>
		void Init(ID3D11Device* device,
				  const vector<VertexT>& vertices,
				  const vector<u32>& indices,
				  const vector<Material>& _materials,
				  const vector<Group>& groups);

		~ModelBlueprint() = default;


	public:
		Mesh mesh;
		AABB aabb;
		u32  group_count;

		vector<Material>  materials;
		vector<ModelPart> model_parts;
};


template<typename VertexT>
void ModelBlueprint::Init(ID3D11Device* device,
						  const vector<VertexT>& vertices,
						  const vector<u32>& indices,
						  const vector<Material>& _materials,
						  const vector<Group>& groups) {

	materials   = _materials;
	group_count = static_cast<u32>(groups.size());

	// Create the mesh
	mesh.Init(device, vertices, indices);

	// Create the AABB for the model
	auto pair = MinMaxPoint(vertices);
	aabb = AABB(pair.first, pair.second);

	for (size_t i = 0; i < group_count; ++i) {
		ModelPart temp;

		temp.name = groups[i].name;
		temp.index_start = groups[i].index_start;
		temp.material_index = groups[i].material_index;

		// Index count
		if (i == group_count - 1)
			temp.index_count = static_cast<u32>(indices.size() - temp.index_start);
		else
			temp.index_count = groups[i + 1].index_start - temp.index_start;


		// Create the AABB for the model part
		vector<VertexT> subvec;

		for (size_t j = temp.index_start; j < (temp.index_start + temp.index_count); ++j) {
			subvec.push_back(vertices[indices[j]]);
		}

		auto pair = MinMaxPoint(subvec);
		temp.aabb = AABB(pair.first, pair.second);

		model_parts.push_back(temp);
	}
}