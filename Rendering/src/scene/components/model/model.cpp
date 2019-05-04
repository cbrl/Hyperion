#include "model.h"


Model::Model(ID3D11Device& device,
             const std::shared_ptr<render::ModelBlueprint>& bp,
             u32 bp_index)
	: Model(device,
	        bp->meshes.at(bp_index).getName(),
            bp->meshes.at(bp_index),
            bp->materials.at(bp->mat_indices.at(bp_index)),
            bp->aabbs.at(bp_index),
            bp->bounding_spheres.at(bp_index),
            bp) {
}


Model::Model(ID3D11Device& device,
             const std::string& name,
             render::Mesh& mesh,
             render::Material& mat,
             AABB aabb,
             BoundingSphere sphere,
             const std::shared_ptr<render::ModelBlueprint>& bp)
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

	render::ModelBuffer buffer_data;

	buffer_data.world               = world_t;
	buffer_data.world_inv_transpose = world_inv_t;
	buffer_data.texTransform        = XMMatrixIdentity();

	buffer_data.mat.base_color = material.params.base_color;
	buffer_data.mat.metalness  = material.params.metalness;
	buffer_data.mat.roughness  = material.params.roughness;
	buffer_data.mat.emissive   = material.params.emissive;

	buffer.updateData(device_context, buffer_data);
}
