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




class ModelRoot final : public Component<ModelRoot> {
	
public:
	//----------------------------------------------------------------------------------
	// Model Node
	//----------------------------------------------------------------------------------
	struct Node final {
		friend class ModelRoot;

	public:
		template<typename ActionT>
		void forEachModel(const ActionT& act) {
			for (auto& model : models) {
				act(model);
			}
		}

		template<typename ActionT>
		void forEachModel(const ActionT& act) const {
			for (const auto& model : models) {
				act(model);
			}
		}

		const std::string& getName() const noexcept {
			return name;
		}

	private:
		// The node's name
		std::string name;

		// The models in the node
		std::vector<Model> models;

		// The children of the node
		std::vector<Node> child_nodes;
	};


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


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	// Apply an action to each node
	template<typename ActionT>
	void forEachNode(const ActionT& act) {
		forEachNodeImpl(act, root);
	}

	// Apply an action to each node
	template<typename ActionT>
	void forEachNode(const ActionT& act) const {
		forEachNodeImpl(act, root);
	}

	// Apply an action to each model ( shortcut for forEachNode(forEachModel(act)) )
	template<typename ActionT>
	void forEachModel(const ActionT& act) {
		forEachModelImpl(act, root);
	}

	// Apply an action to each model ( shortcut for forEachNode(forEachModel(act)) )
	template<typename ActionT>
	void forEachModel(const ActionT& act) const {
		forEachModelImpl(act, root);
	}


private:
	//----------------------------------------------------------------------------------
	// Member Functions - Construction
	//----------------------------------------------------------------------------------

	// Construct the nodes described by the blueprint nodes
	void constructNodes(ID3D11Device& device, ModelBlueprint::Node& bp_current, Node& current);


	//----------------------------------------------------------------------------------
	// Member Functions - Updating
	//----------------------------------------------------------------------------------

	// Update the model buffers of a node and any child nodes
	void XM_CALLCONV updateNodeBuffers(ID3D11DeviceContext& device_context,
                                       Node& current,
                                       FXMMATRIX world_transpose,
                                       CXMMATRIX world_inv_transpose);


	//----------------------------------------------------------------------------------
	// Member Functions - Iteration
	//----------------------------------------------------------------------------------

	template<typename ActionT>
	void forEachModelImpl(const ActionT& act, Node& node) {
		for (auto& child : node.child_nodes) {
			child.forEachModel(act);
			forEachModelImpl(act, child);
		}
	}

	template<typename ActionT>
	void forEachModelImpl(const ActionT& act, const Node& node) const {
		for (const auto& child : node.child_nodes) {
			child.forEachModel(act);
			forEachModelImpl(act, child);
		}
	}

	template<typename ActionT>
	void forEachNodeImpl(const ActionT& act, Node& node) {
		for (auto& child : node.child_nodes) {
			act(child);
			forEachNodeImpl(act, child);
		}
	}

	template<typename ActionT>
	void forEachNodeImpl(const ActionT& act, const Node& node) const {
		for (const auto& child : node.child_nodes) {
			act(child);
			forEachNodeImpl(act, child);
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
	Node root;
};
