#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "directx/vertex_types.h"
#include "directx/directx_math.h"
#include "resource/resource_mgr.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_output.h"
#include "resource/model/material/material.h"
#include "geometry/bounding_volume/bounding_volume.h"


class ResourceMgr;


class ModelBlueprint final : public Resource<ModelBlueprint> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ModelBlueprint(ID3D11Device& device,
	               ResourceMgr& resource_mgr,
	               const std::wstring& filename);

	template<typename VertexT>
	ModelBlueprint(ID3D11Device& device,
	               const ModelOutput<VertexT>& out)
		: Resource(str2wstr(out.name)) {

		constructBlueprint(device, out);
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
	void constructBlueprint(ID3D11Device& device, const ModelOutput<VertexT>& out);


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::string name;

	AABB aabb;
	BoundingSphere sphere;

	shared_ptr<Mesh> mesh;

	vector<Material> materials;

	vector<ModelPart> model_parts;
};


#include "model_blueprint.tpp"
