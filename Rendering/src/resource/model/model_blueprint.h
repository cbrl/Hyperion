#pragma once

#include "directx/d3d11.h"
#include "directx/vertex_types.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_output.h"
#include "geometry/bounding_volume/bounding_volume.h"

class ResourceMgr;

class ModelBlueprint final : public Resource<ModelBlueprint> {
public:
	using Node = ModelOutput::Node;


	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ModelBlueprint(ID3D11Device& device,
	               ResourceMgr& resource_mgr,
	               const std::wstring& filename);

	template<typename VertexT>
	ModelBlueprint(ID3D11Device& device, const ModelOutput& output)
	    : Resource(fs::exists(output.file) ? output.file.wstring() : StrToWstr(output.name)) {

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

		name = out.file.filename().string();

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
			meshes.emplace_back(device, /*mesh.name,*/ vertices, mesh.indices);

			// Construct bounding volumes
			auto [min, max] = MinMaxPoint(mesh.positions);
			auto center     = (min + max) / 2;
			auto radius     = XMVectorGetX(XMVector3Length(XMLoad(&max) - XMLoad(&min)));

			aabbs.emplace_back(min, max);
			bounding_spheres.emplace_back(center, radius);
		}
	}


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::string name;
	std::vector<Mesh> meshes;
	std::vector<AABB> aabbs;
	std::vector<BoundingSphere> bounding_spheres;
	std::vector<Material> materials;
	std::vector<u32> mat_indices;
	Node root;
};
