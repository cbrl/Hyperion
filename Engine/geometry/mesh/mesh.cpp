#pragma once

#include "stdafx.h"
#include "Mesh.h"


template<typename VertexT>
Mesh::Mesh(ID3D11Device* device, vector<VertexT>& vertices, vector<ULONG>& indices, vector<UINT>& groupVertexIndices,
                    vector<Material>& materials, vector<UINT>& groupMaterialIndices) :
	materials(materials),
	groupVertexIndices(groupVertexIndices),
	groupMaterialIndices(groupMaterialIndices),
	vertexCount(vertices.size()),
	indexCount(indices.size())
{
	Init<VertexT>(device, vertices, indices);
}


template<typename VertexT>
void Mesh::Init(ID3D11Device* device, vector<VertexT>& vertices, vector<ULONG>& indices) {
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	D3D11_BUFFER_DESC indexBufferDesc  = {};

	D3D11_SUBRESOURCE_DATA vertexData = {};
	D3D11_SUBRESOURCE_DATA indexData  = {};

	// Vertex buffer description
	vertexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth      = sizeof(VertexT) * vertexCount;
	vertexBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags      = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem          = const_cast<VertexT*>(vertices.data());
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	// Index buffer description
	indexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth      = sizeof(ULONG) * indexCount;
	indexBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags      = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem          = const_cast<ULONG*>(indices.data());
	indexData.SysMemPitch      = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()));
}
