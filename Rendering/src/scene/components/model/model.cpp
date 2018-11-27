#include "model.h"


ModelRoot::ModelRoot(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& bp)
	: name(WstrToStr(bp->guid))
	, blueprint(bp) {

	constructNodes(device, blueprint->root, root);
}


void ModelRoot::constructNodes(ID3D11Device& device, ModelBlueprint::Node& bp_current, ModelNode& current) {
	
	current.name = bp_current.name;

	for (const u32 index : bp_current.mesh_indices) {
		Model model(device,
		            blueprint->meshes[index],
		            blueprint->materials[blueprint->mat_indices[index]],
		            blueprint->aabbs[index],
		            blueprint->bounding_spheres[index]);

		//model.name = node.meshes[index].name;
		current.models.push_back(std::move(model));
	}

	for (auto& bp_node : bp_current.child_nodes) {
		constructNodes(device, bp_node, current.child_nodes.emplace_back());
	}
}


void XM_CALLCONV ModelRoot::updateBuffers(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world) {

	// The model-to-world matrix. Transposed for HLSL.
	const auto world_t = XMMatrixTranspose(object_to_world);

	// Create the inverse transpose of the model-to-world matrix
	const auto world_inv_t = XMMatrixInverse(nullptr, object_to_world);

	updateNodeBuffers(device_context, root, world_t, world_inv_t);
}


void XM_CALLCONV ModelRoot::updateNodeBuffers(ID3D11DeviceContext& device_context,
                                              ModelNode& current,
                                              FXMMATRIX world_transpose,
                                              CXMMATRIX world_inv_transpose) {
	
	for (auto& model : current.models) {
		ModelBuffer buffer_data;
		buffer_data.world               = world_transpose;
		buffer_data.world_inv_transpose = world_inv_transpose;
		buffer_data.texTransform        = XMMatrixIdentity();

		buffer_data.mat = MaterialBuffer{
			model.material.params.ambient,
			model.material.params.diffuse,
			model.material.params.specular,
			model.material.params.emissive,
			model.material.params.spec_scale,
			model.material.params.spec_exponent,
			model.material.params.opacity,
			model.material.params.refraction_index,
			model.material.maps.diffuse != nullptr/*,
			model.material->params.reflection_enabled*/
		};

		model.buffer.updateData(device_context, buffer_data);
	}

	for (auto& node : current.child_nodes) {
		updateNodeBuffers(device_context, node, world_transpose, world_inv_transpose);
	}
}