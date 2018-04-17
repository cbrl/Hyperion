#include "stdafx.h"
#include "model.h"


//----------------------------------------------------------------------------------
// ModelChild
//----------------------------------------------------------------------------------

void XM_CALLCONV ModelChild::Update(ID3D11DeviceContext* device_context,
									FXMMATRIX world,
									CXMMATRIX world_inv_transpose,
									CXMMATRIX world_view_proj) {

	// Create a new ModelBuffer struc with the updated data
	// and send it to the constant buffer.
	ModelBuffer buffer_data;

	buffer_data.world = world;
	buffer_data.world_inv_transpose = world_inv_transpose;
	buffer_data.world_view_proj = world_view_proj;
	buffer_data.texTransform = XMMatrixIdentity();

	buffer_data.mat = MaterialBuffer(material.ambient,
									 material.diffuse,
									 material.specular,
									 material.reflect,
									 material.emissive,
									 material.optical_density,
									 material.dissolve,
									 material.has_texture,
									 material.reflection_enabled);

	buffer.UpdateData(device_context, buffer_data);
}



//----------------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------------

Model::Model(ID3D11Device* device, shared_ptr<ModelBlueprint> blueprint)
	: name(blueprint->name)
	, mesh(blueprint->mesh)
	, aabb(blueprint->aabb)
	, sphere(blueprint->sphere)
{
	// Create each model part
	for (ModelPart& part : blueprint->model_parts) {

		auto& material = blueprint->materials[part.material_index];

		child_models.push_back(ModelChild(device, part, material));
	}
}


void XM_CALLCONV Model::Update(ID3D11DeviceContext* device_context, FXMMATRIX view, CXMMATRIX proj) {

	// Update the object's transform
	transform.Update();

	// Create the model-to-world matrix. Transposed for HLSL.
	auto world = XMMatrixTranspose(transform.GetWorld());

	// Create the inverse transpose of the model-to-world matrix
	auto world_inv_transpose = XMMatrixInverse(NULL, transform.GetWorld());

	// Create the world-view-projection matrix
	auto world_view_proj = XMMatrixTranspose(transform.GetWorld() * view * proj);


	// Update each child model
	ForEachChild([&](ModelChild& child) {

		if (update_bounding_volumes)
			child.UpdateBoundingVolumes(transform.GetWorld());

		child.Update(device_context, world, world_inv_transpose, world_view_proj);
	});

	// Update the model's bounding volumes
	if (update_bounding_volumes) {
		aabb.Transform(transform.GetWorld());
		sphere.Transform(transform.GetWorld());
	}


	// Reset the update flag
	update_bounding_volumes = false;
}