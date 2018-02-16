#include "stdafx.h"
#include "Model.h"


Model::Model(const Mesh& mesh, const AABB& aabb) :
	position(XMMatrixTranslation(0.0f, 0.0f, 0.0f)),
	rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)),
	mesh(mesh),
	aabb(aabb)
{
}


Model::~Model() {
}


void Model::Draw(ID3D11DeviceContext* device_context) {
	UINT offset = 0;

	// Set vertex buffer to active in the input assembler so it can be rendered
	device_context->IASetVertexBuffers(0, 1, mesh.GetVertexBuffer(), &mesh.stride, &offset);

	// Set index buffer to active in the input assembler so it can be rendered
	device_context->IASetIndexBuffer(mesh.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (UINT i = 0; i < mesh.group_count; ++i) {
		auto& material = mesh.materials[mesh.group_material_indices[i]];

		if (material.map_Kd) {
			device_context->PSSetShaderResources(0, 1, material.map_Kd.GetAddressOf());
		}

		// Draw the model
		if (mesh.new_group_indices.size()-1 > i)
			device_context->DrawIndexed(mesh.new_group_indices[i+1] - mesh.new_group_indices[i], mesh.new_group_indices[i], 0);
		else
			device_context->DrawIndexed(mesh.index_count - mesh.new_group_indices[i], mesh.new_group_indices[i], 0);
	}
}