#pragma once

#include <vector>
#include <wrl\client.h>
#include <d3d11.h>
#include "material\material.h"

using std::vector;
using Microsoft::WRL::ComPtr;


class Mesh {
	public:
		Mesh() = default;
		~Mesh() = default;

		template<typename VertexT>
		Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices);

		template<typename VertexT>
		void Init(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices);

		void Draw(ID3D11DeviceContext* device_context, unsigned int index_count, unsigned int start_index) const {
			UINT offset = 0;

			// Set vertex buffer to active in the input assembler so it can be rendered
			device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
			
			// Set index buffer to active in the input assembler so it can be rendered
			device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			
			// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
			device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			device_context->DrawIndexed(index_count, start_index, 0);
		}


	public:
		ComPtr<ID3D11Buffer> vertex_buffer;
		ComPtr<ID3D11Buffer> index_buffer;

		UINT vertex_count;
		UINT index_count;
		UINT stride;
};


template<typename VertexT>
Mesh::Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices)
	: vertex_count(static_cast<uint32_t>(vertices.size()))
	, index_count(static_cast<uint32_t>(indices.size()))
	, stride(sizeof(VertexT))
{
	Init(device, vertices, indices);
}


template<typename VertexT>
void Mesh::Init(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices) {
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	D3D11_BUFFER_DESC indexBufferDesc = {};

	D3D11_SUBRESOURCE_DATA vertexData = {};
	D3D11_SUBRESOURCE_DATA indexData = {};

	// Vertex buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexT) * vertex_count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = const_cast<VertexT*>(vertices.data());
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexData, vertex_buffer.GetAddressOf()));

	// Index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * index_count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = const_cast<UINT*>(indices.data());
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, index_buffer.GetAddressOf()));
}