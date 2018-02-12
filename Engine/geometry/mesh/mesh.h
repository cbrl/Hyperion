#pragma once

#include <vector>
#include <wrl\client.h>
#include <d3d11.h>
#include "material\Material.h"

using std::vector;
using Microsoft::WRL::ComPtr;


class Mesh {
	public:
		Mesh() {}
		template<typename VertexT>
		Mesh(ID3D11Device* device, vector<VertexT>& vertices, vector<UINT>& indices, vector<Material>& materials,
		     UINT groupCount, vector<UINT>& groupVertexIndices, vector<UINT>& groupMaterialIndices);
		~Mesh() {
		}

		template<typename VertexT>
		void Init(ID3D11Device* device, vector<VertexT>& vertices, vector<UINT>& indices);

		ID3D11Buffer** GetVertexBuffer() { return vertexBuffer.GetAddressOf(); }
		ID3D11Buffer* GetIndexBuffer() { return indexBuffer.Get(); }


	public:
		UINT vertexCount;
		UINT indexCount;
		UINT groupCount;
		UINT stride;

		ComPtr<ID3D11Buffer> vertexBuffer;
		ComPtr<ID3D11Buffer> indexBuffer;

		vector<Material> materials;

		vector<UINT> groupVertexIndices;
		vector<UINT> groupMaterialIndices;
};


template<typename VertexT>
Mesh::Mesh(ID3D11Device* device, vector<VertexT>& vertices, vector<UINT>& indices, vector<Material>& materials,
           UINT groupCount, vector<UINT>& groupVertexIndices, vector<UINT>& groupMaterialIndices) :
	materials(materials),
	groupVertexIndices(groupVertexIndices),
	groupMaterialIndices(groupMaterialIndices),
	vertexCount(vertices.size()),
	indexCount(indices.size()),
	groupCount(groupCount),
	stride(sizeof(VertexT))
{
	Init<VertexT>(device, vertices, indices);
}


template<typename VertexT>
void Mesh::Init(ID3D11Device* device, vector<VertexT>& vertices, vector<UINT>& indices) {
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	D3D11_BUFFER_DESC indexBufferDesc = {};

	D3D11_SUBRESOURCE_DATA vertexData = {};
	D3D11_SUBRESOURCE_DATA indexData = {};

	// Vertex buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexT) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = const_cast<VertexT*>(vertices.data());
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf()));

	// Index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = const_cast<UINT*>(indices.data());
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	DX::ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()));
}

//#include "Mesh.tpp"