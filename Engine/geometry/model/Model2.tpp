#pragma once

#include "stdafx.h"
#include "model2.h"


template<typename VertexT>
HRESULT Model<VertexT>::Init(ID3D11Device* device, vector<VertexT> vertices, vector<ULONG> indices,
							 ComPtr<ID3D11ShaderResourceView> modelTexture, ShaderTypes shaderType) {
	HRESULT hr;

	hr = CreateBuffers(device);
	if (FAILED(hr)) {
		return hr;
	}

	texture = modelTexture;
	shader = shaderType;

	return S_OK;
}


template<typename VertexT>
HRESULT Model<VertexT>::CreateBuffers(ID3D11Device* device) {
	HRESULT hr;

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
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

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
	hr = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());

	return hr;
}

template<typename VertexT>
void Model<VertexT>::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	UINT stride = sizeof(VertexT);
	UINT offset = 0;

	// Set vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}