#pragma once

#include "directx/d3d11.h"
#include "directx/vertex/vertex_types.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_config.h"
#include "resource/model/model_output.h"
#include "geometry/bounding_volume/bounding_volume.h"

class ResourceMgr;

class ModelBlueprint final : public Resource<ModelBlueprint> {
public:
	using Node = ModelOutput::Node;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	template<typename VertexT>
	ModelBlueprint(ID3D11Device& device,
	               ResourceMgr& resource_mgr,
	               const std::wstring& filename,
	               const ModelConfig<VertexT>& config);

	template<typename VertexT>
	ModelBlueprint(ID3D11Device& device,
	               const ModelOutput& output,
	               const ModelConfig<VertexT>& config);

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
	void constructBlueprint(ID3D11Device& device, const ModelOutput& out);


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


#include "model_blueprint.tpp"