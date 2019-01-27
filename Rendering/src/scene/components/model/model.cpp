#include "model.h"


Model::Model(ID3D11Device& device,
             const std::string& name,
             Mesh& mesh,
             Material& mat,
             AABB aabb,
             BoundingSphere sphere,
             const std::shared_ptr<ModelBlueprint>& bp)
    : buffer(device)
    , name(name)
    , mesh(mesh)
    , material(mat)
    , aabb(aabb)
    , bounding_sphere(sphere)
    , shadows(true)
	, blueprint(bp) {
}

void XM_CALLCONV Model::updateBuffer(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world) {

	// The model-to-world matrix. Transposed for HLSL.
	const auto world_t = XMMatrixTranspose(object_to_world);

	// Create the inverse transpose of the model-to-world matrix
	const auto world_inv_t = XMMatrixInverse(nullptr, object_to_world);

	ModelBuffer buffer_data;

	buffer_data.world               = world_t;
	buffer_data.world_inv_transpose = world_inv_t;
	buffer_data.texTransform        = XMMatrixIdentity();

	buffer_data.mat.base_color = material.params.base_color;
	buffer_data.mat.metalness  = material.params.metalness;
	buffer_data.mat.roughness  = material.params.roughness;
	buffer_data.mat.emissive   = material.params.emissive;

	buffer.updateData(device_context, buffer_data);
}


/*
ModelRoot::ModelRoot(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& bp)
	: name(bp->name)
	, blueprint(bp) {

	root.constructNode(device, *blueprint, blueprint->root);
}


void XM_CALLCONV ModelRoot::updateBuffers(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world) {

	// The model-to-world matrix. Transposed for HLSL.
	const auto world_t = XMMatrixTranspose(object_to_world);

	// Create the inverse transpose of the model-to-world matrix
	const auto world_inv_t = XMMatrixInverse(nullptr, object_to_world);

	root.updateNodeBuffers(device_context, world_t, world_inv_t);
}


void ModelNode::constructNode(ID3D11Device& device, ModelBlueprint& bp, ModelBlueprint::Node& bp_node) {
	
	name = bp_node.name;

	for (const u32 index : bp_node.mesh_indices) {
		Model model(device,
		            bp.meshes.at(index),
		            bp.materials.at(bp.mat_indices.at(index)),
		            bp.aabbs.at(index),
		            bp.bounding_spheres.at(index));

		model.name = bp.meshes.at(index).getName();
		models.push_back(std::move(model));
	}

	for (auto& node : bp_node.child_nodes) {
		child_nodes.emplace_back().constructNode(device, bp, node);
	}
}


void XM_CALLCONV ModelNode::updateNodeBuffers(ID3D11DeviceContext& device_context,
	                                          FXMMATRIX world_transpose,
	                                          CXMMATRIX world_inv_transpose) {
	
	for (auto& model : models) {
		ModelBuffer buffer_data;

		buffer_data.world               = world_transpose;
		buffer_data.world_inv_transpose = world_inv_transpose;
		buffer_data.texTransform        = XMMatrixIdentity();

		buffer_data.mat.base_color = model.material.params.base_color;
		buffer_data.mat.metalness  = model.material.params.metalness;
		buffer_data.mat.roughness  = model.material.params.roughness;
		buffer_data.mat.emissive   = model.material.params.emissive;

		model.buffer.updateData(device_context, buffer_data);
	}

	for (auto& node : child_nodes) {
		node.updateNodeBuffers(device_context, world_transpose, world_inv_transpose);
	}
}
*/