#include "importer/model_importer.h"

namespace render {

template <typename VertexT>
ModelBlueprint::ModelBlueprint(ID3D11Device& device,
                               ResourceMgr& resource_mgr,
                               const std::wstring& filename,
                               const ModelConfig<VertexT>& config)
	: Resource(filename) {

	const auto out = importer::ImportModel(resource_mgr, filename, config);
	constructBlueprint<VertexT>(device, out);
}

template <typename VertexT>
ModelBlueprint::ModelBlueprint(ID3D11Device& device,
                               const ModelOutput& output,
                               const ModelConfig<VertexT>& config)
    : Resource(output.file.empty() ? StrToWstr(output.name) : output.file.wstring()) {

	constructBlueprint<VertexT>(device, output);
}

template <typename VertexT>
void ModelBlueprint::constructBlueprint(ID3D11Device& device, const ModelOutput& out) {

	name = out.name;

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
				if (not mesh.normals.empty())
					vert.normal = mesh.normals[i];
			}
			if constexpr (VertexT::hasTexture()) {
				if (not mesh.texture_coords.empty())
					vert.texCoord = mesh.texture_coords[i];
			}
			if constexpr (VertexT::hasColor()) {
				if (not mesh.colors.empty())
					vert.color = mesh.colors[i];
			}
			vertices.push_back(std::move(vert));
		}

		// Create the mesh
		meshes.emplace_back(device, mesh.name, vertices, mesh.indices);

		// Construct bounding volumes
		aabbs.emplace_back(AABB::createFromVertices(mesh.positions));
		bounding_spheres.emplace_back(BoundingSphere::createFromVertices(mesh.positions));
	}
}

} //namespace render