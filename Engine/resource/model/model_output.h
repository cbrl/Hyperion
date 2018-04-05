#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "resource\material\material.h"


struct Group {
	string  name;
	u32     index_start;
	u32     material_index;
};


// A ModelOuput object is returned by a model loader and
// used in the construction of a ModelBlueprint.
template<typename VertexT>
struct ModelOutput final {
	public:
		ModelOutput(const string& name,
					const vector<VertexT>& vertices,
					const vector<u32>& indices,
					const vector<Material>& materials,
					const vector<Group>& groups)
			: name(name)
			, vertices(vertices)
			, indices(indices)
			, materials(materials)
			, groups(groups)
		{}

		~ModelOutput() = default;


	public:
		string           name;
		vector<VertexT>  vertices;
		vector<u32>      indices;
		vector<Material> materials;
		vector<Group>    groups;
};
