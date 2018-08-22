#include "model.h"


Model::Model(ID3D11Device& device,
             shared_ptr<Mesh> mesh,
             const ModelPart& part,
             const Material& mat)
	: name(part.name)
	, buffer(device)
	, mesh(std::move(mesh))
	, index_start(part.index_start)
	, index_count(part.index_count)
	, material(mat)
	, aabb(part.aabb)
	, sphere(part.sphere)
	, shadows(true) {
}


void XM_CALLCONV Model::updateBuffer(ID3D11DeviceContext& device_context, FXMMATRIX object_to_world) {

	// Create the model-to-world matrix. Transposed for HLSL.
	auto world_t = XMMatrixTranspose(object_to_world);

	// Create the inverse transpose of the model-to-world matrix
	auto world_inv_transpose = XMMatrixInverse(nullptr, object_to_world);


	// Create a new ModelBuffer struct with the updated data
	// and send it to the constant buffer.
	ModelBuffer buffer_data;

	buffer_data.world               = world_t;
	buffer_data.world_inv_transpose = world_inv_transpose;
	buffer_data.texTransform        = XMMatrixIdentity();

	buffer_data.mat = MaterialBuffer{ material.ambient,
	                                  material.diffuse,
	                                  material.specular,
	                                  material.reflect,
	                                  material.emissive,
	                                  material.optical_density,
	                                  material.has_texture,
	                                  material.reflection_enabled };

	buffer.updateData(device_context, buffer_data);
}
