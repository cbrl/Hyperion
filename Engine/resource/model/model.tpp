#pragma once


template<typename VertexT>
Model::Model(ID3D11Device* device, ModelBlueprint blueprint)
	: position(XMMatrixTranslation(0.0f, 0.0f, 0.0f))
	, rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
	, scale(XMMatrixScaling(1.0f, 1.0f, 1.0f))
	, materials(materials)
{
	Init(device, blueprint);
}


template<typename VertexT>
void Model::Init(ID3D11Device* device, ModelBlueprint blueprint) {
	// Create the mesh
	mesh = Mesh(device, blueprint.vertices, blueprint.indices);

	// Create the AABB for the model
	auto pair = MinMaxPoint(blueprint.vertices);
	aabb = AABB(pair.first, pair.second);

	for (size_t i = 0; i < group_count; ++i) {
		ModelPart temp;

		temp.index_start = blueprint.group_indices[i];
		temp.material_index = blueprint.material_indices[i];


		// Index count
		if (i == group_count - 1)
			temp.index_count = static_cast<u32>(blueprint.indices.size() - blueprint.group_indices[i]);
		else
			temp.index_count = blueprint.group_indices[i + 1] - blueprint.group_indices[i];


		// Create the AABB for the model part
		vector<VertexT> subvec;

		for (size_t j = temp.index_start; j < (temp.index_start + temp.index_count); ++j) {
			subvec.push_back(blueprint.vertices[blueprint.indices[j]]);
		}

		auto pair = MinMaxPoint(subvec);
		temp.aabb = AABB(pair.first, pair.second);

		model_parts.push_back(temp);
	}
}
