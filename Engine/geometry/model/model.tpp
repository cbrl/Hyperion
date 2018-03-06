#pragma once


template<typename VertexT>
Model::Model(ID3D11Device* device, const vector<VertexT>& vertices, const vector<u32>& indices,
             const vector<Material>& materials, const vector<u32>& material_indices,
             u32 group_count, const vector<u32>& group_indices)
	: position(XMMatrixTranslation(0.0f, 0.0f, 0.0f))
	, rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
	, scale(XMMatrixScaling(1.0f, 1.0f, 1.0f))
	, materials(materials)
{
	Init(device, vertices, indices, materials, material_indices, group_count, group_indices);
}


template<typename VertexT>
void Model::Init(ID3D11Device* device, const vector<VertexT>& vertices, const vector<u32>& indices,
                 const vector<Material>& materials, const vector<u32>& material_indices,
                 u32 group_count, const vector<u32>& group_indices)
{
	// Create the mesh
	mesh = Mesh(device, vertices, indices);

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
