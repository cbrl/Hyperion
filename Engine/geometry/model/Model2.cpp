#include "stdafx.h"
#include "Model2.h"


Model::Model(const Mesh& mesh, const AABB& aabb) :
	position(XMMatrixTranslation(0.0f, 0.0f, 0.0f)),
	rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)),
	mesh(mesh),
	aabb(aabb)
{
}


Model::~Model() {
}


void Model::Draw(ID3D11DeviceContext* deviceContext) {
	UINT offset = 0;

	// Set vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, mesh.GetVertexBuffer(), &mesh.stride, &offset);

	// Set index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(mesh.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (UINT i = 0; i < mesh.groupCount; ++i) {
		auto& material = mesh.materials[mesh.groupMaterialIndices[i]];

		if (material.map_Kd) {
			deviceContext->PSSetShaderResources(0, 1, material.map_Kd.GetAddressOf());
		}

		// Draw the model
		if (mesh.newGroupIndices.size()-1 > i)
			deviceContext->DrawIndexed(mesh.newGroupIndices[i+1] - mesh.newGroupIndices[i], mesh.newGroupIndices[i], 0);
		else
			deviceContext->DrawIndexed(mesh.indexCount - mesh.newGroupIndices[i], mesh.newGroupIndices[i], 0);
	}
}