#include "loader/model_loader.h"

template <typename VertexT>
ModelBlueprint::ModelBlueprint(ID3D11Device& device,
                               ResourceMgr& resource_mgr,
                               const std::wstring& filename,
                               const ModelConfig<VertexT>& config)
	: Resource(filename) {

	const auto out = ModelLoader::Load(resource_mgr, filename, config);
	constructBlueprint<VertexT>(device, out);
}

template <typename VertexT>
ModelBlueprint::ModelBlueprint(ID3D11Device& device,
                               const ModelOutput& output,
                               const ModelConfig<VertexT>& config)
	: Resource(fs::exists(output.file) ? output.file.wstring() : StrToWstr(output.name)) {

	constructBlueprint<VertexT>(device, output);
}

template <typename VertexT>
void ModelBlueprint::constructBlueprint(ID3D11Device& device, const ModelOutput& out) {

	name = out.file.empty() ? out.name : out.file.filename().string();

	// Copy the nodes and materials
	root = out.root;
	materials = out.materials;

	// Create the model data
	for (const auto& mesh : out.meshes) {

		// Material index
		mat_indices.push_back(mesh.material_index);

		// Construct vertices
		std::vector<VertexT> vertices;
		for (size_t i = 0; i < mesh.positions.size(); ++i) {
			VertexT vert;
			vert.position = mesh.positions[i];
			if constexpr (VertexT::hasNormal()) {
				if (!mesh.normals.empty())
					vert.normal = mesh.normals[i];
			}
			if constexpr (VertexT::hasTexture()) {
				if (!mesh.texture_coords.empty())
					vert.texCoord = mesh.texture_coords[i];
			}
			if constexpr (VertexT::hasColor()) {
				if (!mesh.colors.empty())
					vert.color = mesh.colors[i];
			}
			vertices.push_back(std::move(vert));
		}

		// Create the mesh
		meshes.emplace_back(device, mesh.name, vertices, mesh.indices);

		// Construct bounding volumes
		auto [min, max] = MinMaxPoint(mesh.positions);
		auto center = (min + max) / 2;
		auto radius = XMVectorGetX(XMVector3Length(XMLoad(&max) - XMLoad(&min)));

		aabbs.emplace_back(min, max);
		bounding_spheres.emplace_back(center, radius);
	}
}