#pragma once

#include "util\datatypes\datatypes.h"
#include "util\string\string.h"
#include "util\math\math.h"
#include "geometry\boundingvolume\bounding_volume.h"
#include "resource\material\material.h"


struct ModelPart {
	u32  index_start;
	u32  index_count;
	u32  material_index;
	AABB aabb;
};


template<typename VertexT>
struct ModelBlueprint {
	ModelBlueprint(const vector<VertexT>& vertices, const vector<u32>& indices,
				   const vector<Material>& materials, const vector<u32>& material_indices,
				   u32 group_count = 1, const vector<u32>& group_indices = {});

	~ModelBlueprint() = default;


	// Vertex info
	vector<VertexT> vertices;
	vector<u32>     indices;

	// Material info
	vector<Material> materials;
	vector<u32>      material_indices;

	// Group info
	u32         group_count;
	vector<u32> group_indices;

	// Model parts
	vector<ModelPart> model_parts;

	// AABB
	AABB aabb;
};



template<typename VertexT>
ModelBlueprint<VertexT>::ModelBlueprint(const vector<VertexT>& vertices, const vector<u32>& indices,
										const vector<Material>& materials, const vector<u32>& material_indices,
										u32 group_count, const vector<u32>& group_indices)
	: vertices(vertices)
	, indices(indices)
	, materials(materials)
	, material_indices(material_indices)
	, group_count(group_count)
	, group_indices(group_indices)
{
	// Create the AABB for the model
	auto pair = MinMaxPoint(vertices);
	aabb = AABB(pair.first, pair.second);

	for (size_t i = 0; i < group_count; ++i) {
		ModelPart temp;

		temp.index_start = group_indices[i];
		temp.material_index = material_indices[i];


		// Index count
		if (i == group_count - 1)
			temp.index_count = static_cast<u32>(indices.size() - group_indices[i]);
		else
			temp.index_count = group_indices[i + 1] - group_indices[i];


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