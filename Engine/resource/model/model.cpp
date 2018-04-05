#include "stdafx.h"
#include "model.h"


//----------------------------------------------------------------------------------
// ChildModel
//----------------------------------------------------------------------------------

void XM_CALLCONV ChildModel::Update(ID3D11DeviceContext* device_context,
									FXMMATRIX world,
									CXMMATRIX world_inv_transpose,
									CXMMATRIX world_view_proj,
									CXMMATRIX tex_transform) {

	// Create a new ModelBuffer struc with the updated data
	// and send it to the constant buffer.
	ModelBuffer buffer_data;

	buffer_data.world = world;
	buffer_data.world_inv_transpose = world_inv_transpose;
	buffer_data.world_view_proj = world_view_proj;
	buffer_data.texTransform = tex_transform;

	buffer_data.mat = MaterialBuffer(material->Ka, material->Kd, material->Ks, material->Ke,
									 material->Ni, material->d, material->has_texture);

	buffer.UpdateData(device_context, buffer_data);
}



//----------------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------------

Model::Model(ID3D11Device* device, ModelBlueprint blueprint)
	: name(blueprint.name)
	, mesh(blueprint.mesh)
	, aabb(blueprint.aabb)
	, sphere(blueprint.sphere)
	, position(XMMatrixTranslation(0.0f, 0.0f, 0.0f))
	, rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
	, scale(XMMatrixScaling(1.0f, 1.0f, 1.0f))
	, transform(XMMatrixIdentity())
{
	// Create the shared ptrs for the material vector
	for (Material& mat : blueprint.materials) {
		materials.push_back(make_shared<Material>(mat));
	}

	// Create each model part
	for (ModelPart& part : blueprint.model_parts) {
		child_models.push_back(ChildModel(device, part, materials[part.material_index]));
	}
}


void XM_CALLCONV Model::Update(ID3D11DeviceContext* device_context, FXMMATRIX view, CXMMATRIX proj) {

	// Update the object matrix
	transform = XMMatrixMultiply(transform, (scale*rotation*position));

	// Create the model-to-world matrix. Transposed for HLSL.
	auto world = XMMatrixTranspose(transform);

	// Create the inverse transpose of the model-to-world matrix
	auto world_inv_transpose = XMMatrixInverse(NULL, transform);

	// Create the world-view-projection matrix
	auto world_view_proj = XMMatrixTranspose(transform * view * proj);


	// Update each child model
	ForEachChild([&](ChildModel& child) {

		if (update_bounding_volumes)
			child.UpdateBoundingVolumes(transform);

		child.Update(device_context, world, world_inv_transpose, world_view_proj, XMMatrixIdentity());
	});

	// Update the model's bounding volumes
	if (update_bounding_volumes) {
		aabb.Transform(transform);
		sphere.Transform(transform);
	}


	// Reset the individual transformation matrices and the update flag
	scale = rotation = position = XMMatrixIdentity();
	update_bounding_volumes = false;
}