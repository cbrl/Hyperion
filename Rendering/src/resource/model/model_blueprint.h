#pragma once

#include "directx/d3d11.h"
#include "directx/vertex_types.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_output.h"

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
	ModelBlueprint(ID3D11Device& device, const ModelOutput<VertexT>& model_output)
		: Resource(StrToWstr(model_output.name)) {

		constructBlueprint(device, model_output);
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


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	shared_ptr<Mesh>& getMesh() {
		return mesh;
	}

	const std::vector<Material>& getMaterials() const {
		return materials;
	}

	template<typename ActionT>
	void forEachPart(ActionT act) {
		for (ModelPart& part : model_parts) {
			act(part);
		}
	}


private:
	template<typename VertexT>
	void constructBlueprint(ID3D11Device& device, const ModelOutput<VertexT>& out) {
		// Copy members
		name        = out.name;
		materials   = out.materials;
		model_parts = out.model_parts;

		// Create the mesh
		mesh = std::make_shared<Mesh>(device, out.vertices, out.indices);
	}


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::string name;

	shared_ptr<Mesh> mesh;

	std::vector<Material> materials;

	std::vector<ModelPart> model_parts;
};
