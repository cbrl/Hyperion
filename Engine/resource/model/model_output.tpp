
template<typename VertexT>
ModelOutput<VertexT>::ModelOutput(const string& name,
								  const vector<VertexT>& vertices,
								  const vector<u32>& indices,
								  const vector<Material>& materials,
								  const vector<Group>& groups)
	: name(name)
	, vertices(vertices)
	, indices(indices)
	, materials(materials) {

	size_t group_count = groups.size();

	// Create the AABB
	auto pair = MinMaxPoint(vertices);
	aabb = AABB(pair.first, pair.second);

	// Create the bounding sphere
	auto center = (pair.first + pair.second) / 2;
	auto radius = XMVectorGetX(XMVector3Length(pair.second - pair.first));
	sphere = BoundingSphere(center, radius);

	// Create the model parts
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

		model_parts.push_back(std::move(temp));
	}
}