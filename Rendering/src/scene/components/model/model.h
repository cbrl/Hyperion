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


class Model {
	friend class ModelRoot;

public:
	Model(ID3D11Device& device, Mesh& mesh, Material& mat, AABB aabb, BoundingSphere sphere)
		: buffer(device)
		, mesh(mesh)
		, material(mat)
		, aabb(aabb)
		, bounding_sphere(sphere)
		, shadows(true) {
	}

	//----------------------------------------------------------------------------------
	// Member Functions - Bind
	//----------------------------------------------------------------------------------

	void bindMesh(ID3D11DeviceContext& device_context) const {
		mesh.bind(device_context);
	}

	template<typename StageT>
	void bindBuffer(ID3D11DeviceContext& device_context, u32 slot) const {
		buffer.bind<StageT>(device_context, slot);
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Material
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	u32 getVertexCount() const noexcept {
		return mesh.getVertexCount();
	}

	[[nodiscard]]
	u32 getIndexCount() const noexcept {
		return mesh.getIndexCount();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Material
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	Material& getMaterial() noexcept {
		return material;
	}

	[[nodiscard]]
	const Material& getMaterial() const noexcept {
		return material;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Bounding Volumes
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const AABB& getAABB() const noexcept {
		return aabb;
	}

	[[nodiscard]]
	const BoundingSphere& getBoundingSphere() const noexcept {
		return bounding_sphere;
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Shadows
	//----------------------------------------------------------------------------------

	void setShadows(bool state) noexcept {
		shadows = state;
	}

	[[nodiscard]]
	bool castsShadows() const noexcept {
		return shadows;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	//std::string name;
	ConstantBuffer<ModelBuffer> buffer;
	Mesh& mesh;
	Material& material;
	AABB aabb;
	BoundingSphere bounding_sphere;
	bool shadows;
};


class ModelNode final {
	friend class ModelRoot;

private:
	// The node's name
	std::string name;

	// The models in the node
	std::vector<Model> models;

	// The children of the node
	std::vector<ModelNode> child_nodes;
};


class ModelRoot final : public Component<ModelRoot> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	ModelRoot(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& bp);
	ModelRoot(const ModelRoot& model) = delete;
	ModelRoot(ModelRoot&& mode) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~ModelRoot() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	ModelRoot& operator=(const ModelRoot& model) = delete;
	ModelRoot& operator=(ModelRoot&& model) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	[[nodiscard]]
	const std::string& getName() const noexcept {
		return name;
	}

	// Update the models' cbuffers
	void XM_CALLCONV updateBuffers(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world);

	// Apply an action to each model
	template<typename ActionT>
	void forEachModel(ActionT&& act) {
		forEachModelImpl(act, root);
	}

	// Apply an action to each model
	template<typename ActionT>
	void forEachModel(ActionT&& act) const {
		forEachModelImpl(act, root);
	}


private:
	// Construct the nodes described by the blueprint nodes
	void constructNodes(ID3D11Device& device, ModelBlueprint::Node& bp_current, ModelNode& current);

	// Update the model buffers of a node and any child nodes
	void XM_CALLCONV updateNodeBuffers(ID3D11DeviceContext& device_context,
                                       ModelNode& current,
                                       FXMMATRIX world_transpose,
                                       CXMMATRIX world_inv_transpose);

	template<typename ActionT>
	void forEachModelImpl(const ActionT& act, ModelNode& node) {
		for (auto& model : node.models) {
			act(model);
		}
		for (auto& child : node.child_nodes) {
			forEachModelImpl(act, child);
		}
	}

	template<typename ActionT>
	void forEachModelImpl(const ActionT& act, const ModelNode& node) const {
		for (const auto& model : node.models) {
			act(model);
		}
		for (const auto& child : node.child_nodes) {
			forEachModelImpl(act, child);
		}
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// The model's name
	std::string name;

	// The blueprint whose data is referenced to by the models
	std::shared_ptr<ModelBlueprint> blueprint;

	// The root node
	ModelNode root;
};
