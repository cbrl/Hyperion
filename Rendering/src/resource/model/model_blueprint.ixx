module;

#include <string>
#include <vector>

#include "datatypes/scalar_types.h"

#include "directx/d3d11.h"

export module rendering:model_blueprint;

import math.geometry;

import :resource;
import :mesh;
import :model_config;
import :model_output;
import :vertex_types;


namespace render {

export class ModelBlueprint final : public Resource<ModelBlueprint> {
public:
	using Node = ModelOutput::Node;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	template<typename VertexT>
	ModelBlueprint(ID3D11Device& device,
	               const ModelOutput& output,
	               const ModelConfig<VertexT>& config)
		: Resource(output.file.empty() ? StrToWstr(output.name) : output.file.wstring()) {

		constructBlueprint<VertexT>(device, output);
	}

	ModelBlueprint(const ModelBlueprint& blueprint) = delete;
	ModelBlueprint(ModelBlueprint&& blueprint) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ModelBlueprint() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ModelBlueprint& operator=(const ModelBlueprint& blueprint) = delete;
	ModelBlueprint& operator=(ModelBlueprint&& blueprint) noexcept = default;


private:
	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	template<typename VertexT>
	void constructBlueprint(ID3D11Device& device, const ModelOutput& out) {
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


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Model data
	std::string name;
	std::vector<Mesh> meshes;
	std::vector<AABB> aabbs;
	std::vector<BoundingSphere> bounding_spheres;
	std::vector<Material> materials;
	std::vector<u32> mat_indices;

	// A hierarchy of nodes that define models
	Node root;
};

} //namespace render
