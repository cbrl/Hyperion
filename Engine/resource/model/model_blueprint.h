#pragma once

#include "util\datatypes\datatypes.h"
#include "util\string\string.h"
#include "util\math\math.h"
#include "geometry\boundingvolume\bounding_volume.h"
#include "resource\material\material.h"


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


template<typename VertexT>
struct ModelBlueprint {
	ModelBlueprint(const vector<VertexT>& vertices,
				   const vector<u32>& indices,
				   const vector<Material>& materials,
				   const vector<Group>& groups);

	~ModelBlueprint() = default;


	// Vertex info
	vector<VertexT> vertices;
	vector<u32>     indices;

	// Materials
	vector<Material> materials;

	// Group info
	u32 group_count;

	// Model parts
	vector<ModelPart> model_parts;

	// AABB
	AABB aabb;
};



template<typename VertexT>
ModelBlueprint<VertexT>::ModelBlueprint(const vector<VertexT>& vertices,
										const vector<u32>& indices,
										const vector<Material>& materials,
										const vector<Group>& groups)
	: vertices(vertices)
	, indices(indices)
	, materials(materials)
	, group_count(static_cast<u32>(groups.size()))
{
	// Create the AABB for the model
	auto pair = MinMaxPoint(vertices);
	aabb = AABB(pair.first, pair.second);

	for (size_t i = 0; i < group_count; ++i) {
		ModelPart temp;

		temp.name           = groups[i].name;
		temp.index_start    = groups[i].index_start;
		temp.material_index = groups[i].material_index;

		// Index count
		if (i == group_count - 1)
			temp.index_count = static_cast<u32>(indices.size() - temp.index_start);
		else
			temp.index_count = groups[i+1].index_start - temp.index_start;


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