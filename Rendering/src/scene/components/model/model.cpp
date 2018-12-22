#include "model.h"


Model::Model(ID3D11Device& device,
             Mesh& mesh,
             Material& mat,
             AABB aabb,
             BoundingSphere sphere)
    : buffer(device)
    , mesh(mesh)
    , material(mat)
    , aabb(aabb)
    , bounding_sphere(sphere)
    , shadows(true)
    , active(true) {
}


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

		buffer_data.mat = MaterialBuffer {
			model.material.params.ambient,
			model.material.params.diffuse,
			model.material.params.specular,
			model.material.params.emissive,
			model.material.params.spec_scale,
			model.material.params.spec_exponent,
			model.material.params.opacity,
		    model.material.params.mirror,
			model.material.params.reflectivity,
			model.material.maps.diffuse != nullptr
		};

		model.buffer.updateData(device_context, buffer_data);
	}

	for (auto& node : child_nodes) {
		node.updateNodeBuffers(device_context, world_transpose, world_inv_transpose);
	}
}