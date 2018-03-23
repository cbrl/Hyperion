#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "resource\material\material.h"


struct Group {
	wstring name;
	u32     index_start;
	u32     material_index;
};


template<typename VertexT>
struct ModelOutput {
	public:
		ModelOutput(const wstring& name,
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
		wstring          name;
		vector<VertexT>  vertices;
		vector<u32>      indices;
		vector<Material> materials;
		vector<Group>    groups;
};
