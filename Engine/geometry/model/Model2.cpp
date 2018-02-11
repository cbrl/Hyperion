#include "stdafx.h"
#include "Model2.h"


Model::Model(Mesh mesh) :
	position(XMMatrixTranslation(0.0f, 0.0f, 0.0f)),
	rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)),
	mesh(mesh)
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

	for (int i = 0; i < mesh.groupCount; ++i) {
		if (mesh.materials[mesh.groupMaterialIndices[i]].map_Kd) {
			deviceContext->PSSetShaderResources(0, 1, mesh.materials[mesh.groupMaterialIndices[i]].map_Kd.GetAddressOf());
		}

		// Draw the model
		deviceContext->DrawIndexed(mesh.groupVertexIndices[i+1] - mesh.groupVertexIndices[i], mesh.groupVertexIndices[i], 0);
	}
}