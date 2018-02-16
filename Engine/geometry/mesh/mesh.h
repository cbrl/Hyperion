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
		Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices, const vector<Material>& materials,
		     UINT group_count, const vector<UINT>& new_group_indices, const vector<UINT>& group_material_indices);
		~Mesh() {}

		template<typename VertexT>
		void Init(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices);

		ID3D11Buffer** GetVertexBuffer() { return vertex_buffer.GetAddressOf(); }
		ID3D11Buffer* GetIndexBuffer() { return index_buffer.Get(); }


	public:
		ComPtr<ID3D11Buffer> vertex_buffer;
		ComPtr<ID3D11Buffer> index_buffer;

		vector<Material> materials;

		// Vector of indices where a new group starts
		vector<UINT> new_group_indices;
		// Vector of indices for the material belonging to a group
		vector<UINT> group_material_indices;

		UINT vertex_count;
		UINT index_count;
		UINT group_count;
		UINT stride;
};


template<typename VertexT>
Mesh::Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices, const vector<Material>& materials,
           UINT group_count, const vector<UINT>& new_group_indices, const vector<UINT>& group_material_indices) :
	materials(materials),
	new_group_indices(new_group_indices),
	group_material_indices(group_material_indices),
	vertex_count(vertices.size()),
	index_count(indices.size()),
	group_count(group_count),
	stride(sizeof(VertexT))
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