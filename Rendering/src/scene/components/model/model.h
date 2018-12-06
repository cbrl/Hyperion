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


//----------------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------------
//
// A single model contained in the object hierarchy. Contains a mesh, material,
// bounding volumes, and a shader constant buffer.
//
//----------------------------------------------------------------------------------
class Model {
	friend class ModelNode;
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




//----------------------------------------------------------------------------------
// ModelNode
//----------------------------------------------------------------------------------
//
// A node in the object hierarchy. Can contain multiple models and child nodes, or none.
//
//----------------------------------------------------------------------------------
class ModelNode final {
	friend class ModelRoot;

public:
	// Apply an action to the node's children
	template<typename ActionT>
	void forEachNode(const ActionT& act) {
		for (auto& node : child_nodes) {
			act(node);
		}
	}

	// Apply an action to the node's children
	template<typename ActionT>
	void forEachNode(const ActionT& act) const {
		for (const auto& node : child_nodes) {
			act(node);
		}
	}

	// Apply an action to all descendants of the node recursively
	template<typename ActionT>
	void forEachNodeRecursive(const ActionT& act) {
		for (auto& node : child_nodes) {
			act(node);
			node.forEachNodeRecursive(act);
		}
	}

	// Apply an action to all descendants of the node recursively
	template<typename ActionT>
	void forEachNodeRecursive(const ActionT& act) const {
		for (const auto& node : child_nodes) {
			act(node);
			node.forEachNodeRecursive(act);
		}
	}

	// Apply an action to each model in this node
	template<typename ActionT>
	void forEachModel(const ActionT& act) {
		for (auto& model : models) {
			act(model);
		}
	}

	// Apply an action to each model in this node
	template<typename ActionT>
	void forEachModel(const ActionT& act) const {
		for (const auto& model : models) {
			act(model);
		}
	}

	// Apply an action to each model in this node and all descendants recursively
	template<typename ActionT>
	void forEachModelRecursive(const ActionT& act) {
		forEachModel(act);
		forEachNodeRecursive([&act](ModelNode& node) {
			node.forEachModel(act);
		});
	}

	// Apply an action to each model in this node and all descendants recursively
	template<typename ActionT>
	void forEachModelRecursive(const ActionT& act) const {
		forEachModel(act);
		forEachNodeRecursive([&act](const ModelNode& node) {
			node.forEachModel(act);
		});
	}

	const std::string& getName() const noexcept {
		return name;
	}

private:
	// Recursively construct the nodes described by the blueprint nodes
	void constructNode(ID3D11Device& device, ModelBlueprint& bp, ModelBlueprint::Node& bp_node);

	// Update the model buffers of the node and any child nodes
	void XM_CALLCONV updateNodeBuffers(ID3D11DeviceContext& device_context,
		FXMMATRIX world_transpose,
		CXMMATRIX world_inv_transpose);

private:
	// The node's name
	std::string name;

	// The models in the node
	std::vector<Model> models;

	// The children of the node
	std::vector<ModelNode> child_nodes;
};




//----------------------------------------------------------------------------------
// ModelRoot
//----------------------------------------------------------------------------------
//
// The root node of the object hierarchy, and also a component that can be added
// to an entity. Provides some helper functions for iterating over the nodes
// in the hierarchy.
//
//----------------------------------------------------------------------------------
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

	[[nodiscard]]
	ModelNode& getRootNode() noexcept {
		return root;
	}

	[[nodiscard]]
	const ModelNode& getRootNode() const noexcept {
		return root;
	}

	// Update the cbuffers of all models
	void XM_CALLCONV updateBuffers(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world);


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to all nodes recursively
	template<typename ActionT>
	void forEachNode(const ActionT& act) {
		act(root);
		root.forEachNodeRecursive(act);
	}

	// Apply an action to all nodes recursively
	template<typename ActionT>
	void forEachNode(const ActionT& act) const {
		act(root);
		root.forEachNodeRecursive(act);
	}

	// Apply an action to all models recursively
	template<typename ActionT>
	void forEachModel(const ActionT& act) {
		root.forEachModelRecursive(act);
	}

	// Apply an action to all models recursively
	template<typename ActionT>
	void forEachModel(const ActionT& act) const {
		root.forEachModelRecursive(act);
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
