#pragma once

#include "directx/d3d11.h"
#include "directx/directx_math.h"
#include "datatypes/datatypes.h"
#include "io/io.h"

#include "component/component.h"
#include "buffer/buffers.h"
#include "buffer/constant_buffer.h"
#include "resource/mesh/mesh.h"
#include "resource/model/model_blueprint.h"
#include "resource/model/material/material.h"
#include "geometry/bounding_volume/bounding_volume.h"


class ModelChild final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ModelChild(ID3D11Device& device, ModelPart& part, Material mat)
		: name(part.name)
		, buffer(device)
		, material(std::move(mat))
		, index_start(part.index_start)
		, index_count(part.index_count)
		, aabb(part.aabb)
		, sphere(part.sphere)
		, shadows(true) {
	}

	ModelChild(const ModelChild& child) = delete;
	ModelChild(ModelChild&& child) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ModelChild() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ModelChild& operator=(const ModelChild& child) = delete;
	ModelChild& operator=(ModelChild&& child) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Buffer
	//----------------------------------------------------------------------------------

	// Bind the child model's buffer to a pipeline stage
	template<typename StageT>
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<StageT>(device_context, slot);
	}

	// Update the child model's buffer
	void XM_CALLCONV updateBuffer(ID3D11DeviceContext& device_context,
	                              FXMMATRIX object_to_world,
	                              CXMMATRIX world_inv_transpose) const;


	//----------------------------------------------------------------------------------
	// Member Functions - Index info
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32 getIndexStart() const {
		return index_start;
	}

	[[nodiscard]]
	u32 getIndexCount() const {
		return index_count;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Name
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const std::string& getName() const {
		return name;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Bounding volumes
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const AABB& getAABB() const {
		return aabb;
	}

	[[nodiscard]]
	const BoundingSphere& getSphere() const {
		return sphere;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Material
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	Material& getMaterial() {
		return material;
	}

	[[nodiscard]]
	const Material& getMaterial() const {
		return material;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Shadows
	//----------------------------------------------------------------------------------

	void setShadows(bool state) {
		shadows = state;
	}

	[[nodiscard]]
	bool castsShadows() const {
		return shadows;
	}


private:
	// The name of the model child
	std::string name;

	// The cbuffer for this model child
	ConstantBuffer<ModelBuffer> buffer;

	// The model child's material
	Material material;

	// Index info
	u32 index_start;
	u32 index_count;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;

	// Flag that decides if the model can cast shadows
	bool shadows;
};


class Model final : public Component<Model> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Model(ID3D11Device& device, shared_ptr<ModelBlueprint> blueprint);
	Model(const Model& model) = delete;
	Model(Model&& mode) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Model() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Model& operator=(const Model& model) = delete;
	Model& operator=(Model&& model) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions - Mesh
	//----------------------------------------------------------------------------------

	// Bind the model's vertex and index buffers
	void bind(ID3D11DeviceContext& device_context) const {
		mesh->bind(device_context);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Preform an action for each child model
	template<typename ActionT>
	void forEachChild(ActionT act) {
		for (auto& child : child_models) {
			act(child);
		}
	}

	template<typename ActionT>
	void forEachChild(ActionT act) const {
		for (const auto& child : child_models) {
			act(child);
		}
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Buffer
	//----------------------------------------------------------------------------------

	// Update model matrix and bounding volumes, as well as those of the child models.
	void XM_CALLCONV updateBuffer(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world);


	//----------------------------------------------------------------------------------
	// Member Functions - Misc
	//----------------------------------------------------------------------------------

	const std::string& getName() const { return name; }
	const AABB& getAABB() const { return aabb; }
	const BoundingSphere& getSphere() const { return sphere; }


private:
	// The model's name
	std::string name;

	// The vertex and index buffer of the model
	shared_ptr<Mesh> mesh;

	// Bounding volumes
	AABB aabb;
	BoundingSphere sphere;

	// The child models that make up this model
	vector<ModelChild> child_models;
};
