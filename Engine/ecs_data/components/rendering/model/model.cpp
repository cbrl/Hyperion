#include "stdafx.h"
#include "model.h"


//----------------------------------------------------------------------------------
// ModelChild
//----------------------------------------------------------------------------------

void XM_CALLCONV ModelChild::UpdateBuffer(ID3D11DeviceContext& device_context,
									FXMMATRIX world,
									CXMMATRIX world_inv_transpose,
									CXMMATRIX world_view_proj) {

	// Create a new ModelBuffer struc with the updated data
	// and send it to the constant buffer.
	ModelBuffer buffer_data;

	buffer_data.world               = world;
	buffer_data.world_inv_transpose = world_inv_transpose;
	buffer_data.world_view_proj     = world_view_proj;
	buffer_data.texTransform        = XMMatrixIdentity();

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

Model::Model(ID3D11Device& device, shared_ptr<ModelBlueprint> blueprint)
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


void XM_CALLCONV Model::UpdateBuffer(ID3D11DeviceContext& device_context,
									 FXMMATRIX world,
									 CXMMATRIX view,
									 CXMMATRIX proj) {

	// Create the model-to-world matrix. Transposed for HLSL.
	auto world_t = XMMatrixTranspose(world);

	// Create the inverse transpose of the model-to-world matrix
	auto world_inv_transpose = XMMatrixInverse(NULL, world);

	// Create the world-view-projection matrix
	auto world_view_proj = XMMatrixTranspose(world * view * proj);


	// Update each child model
	ForEachChild([&](ModelChild& child) {
		child.UpdateBuffer(device_context, world_t, world_inv_transpose, world_view_proj);
	});
}


void XM_CALLCONV Model::UpdateBoundingVolumes(FXMMATRIX world) {

	// Update each child model
	ForEachChild([&](ModelChild& child) {
		child.UpdateBoundingVolumes(world);
	});

	// Update the model
	aabb.Transform(world);
	sphere.Transform(world);
}