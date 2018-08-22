template<typename VertexT>
ModelOutput<VertexT>::ModelOutput(const std::string& name,
                                  const std::vector<VertexT>& vertices,
                                  const std::vector<u32>& indices,
                                  const std::vector<Material>& materials,
                                  const std::vector<Group>& groups)
	: name(name)
	, vertices(vertices)
	, indices(indices)
	, materials(materials) {

	// Create the model parts
	for (size_t i = 0; i < groups.size(); ++i) {
		ModelPart temp;

		temp.name           = groups[i].name;
		temp.index_start    = groups[i].index_start;
		temp.material_index = groups[i].material_index;

		// Index count
		if (i == groups.size() - 1)
			temp.index_count = static_cast<u32>(indices.size() - temp.index_start);
		else
			temp.index_count = groups[i+1].index_start - temp.index_start;


		// Create the bounding volumes
		std::vector<VertexT> subvec;
		for (size_t j = temp.index_start; j < (temp.index_start + temp.index_count); ++j) {
			subvec.push_back(vertices[indices[j]]);
		}

		auto [min, max] = MinMaxPoint(subvec);
		auto center     = (min + max) / 2;
		auto radius     = XMVectorGetX(XMVector3Length(XMLoad(&max) - XMLoad(&min)));

		temp.aabb   = AABB{min, max};
		temp.sphere = BoundingSphere{center, radius};


		// Add the model part
		model_parts.push_back(std::move(temp));
	}
}
