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

		/*template<typename VertexT>
		Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices, const vector<Material>& materials,
		     UINT group_count, const vector<UINT>& new_group_indices, const vector<UINT>& group_material_indices);*/

		template<typename VertexT>
		Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices);
		~Mesh() = default;

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


	//public:
	//	struct MeshPart {
	//		unsigned int index_start;
	//		unsigned int index_count;
	//		unsigned int material_index;
	//	};


	public:
		ComPtr<ID3D11Buffer> vertex_buffer;
		ComPtr<ID3D11Buffer> index_buffer;

		//// Vector of indices where a new group starts
		//vector<UINT> new_group_indices;
		//// Vector of indices for the material belonging to a group
		//vector<UINT> group_material_indices;
		//// Vector of descriptors for each group
		//vector<MeshPart> mesh_parts;

		//// Vector of the materials for this mesh
		//vector<Material> materials;

		UINT vertex_count;
		UINT index_count;
		//UINT group_count;
		UINT stride;
};


//template<typename VertexT>
//Mesh::Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<UINT>& indices, const vector<Material>& materials,
//           UINT group_count, const vector<UINT>& new_group_indices, const vector<UINT>& group_material_indices) :
//	materials(materials),
//	new_group_indices(new_group_indices),
//	group_material_indices(group_material_indices),
//	vertex_count(vertices.size()),
//	index_count(indices.size()),
//	group_count(group_count),
//	stride(sizeof(VertexT))
//{
//	Init(device, vertices, indices);
//}


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


	// Create the vector of mesh parts;
	//for (size_t i = 0; i < new_group_indices.size(); ++i) {
	//	MeshPart temp = {};

	//	temp.index_start = indices[new_group_indices[i]];
	//	temp.index_count = new_group_indices[i + 1] - new_group_indices[i];
	//	temp.material_index = group_material_indices[i];

	//	mesh_parts.push_back(temp);
	//}
}