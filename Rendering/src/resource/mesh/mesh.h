#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "pipeline.h"


class Mesh final {
public:
	Mesh() = default;
	~Mesh() = default;

	template<typename VertexT>
	Mesh(ID3D11Device& device, const vector<VertexT>& vertices, const vector<u32>& indices);


	// Bind the vertex buffer and index buffer
	void bind(ID3D11DeviceContext& device_context) const {
		u32 offset = 0;

		// Set vertex buffer to active in the input assembler so it can be rendered
		Pipeline::IA::bindVertexBuffers(device_context, 0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

		// Set index buffer to active in the input assembler so it can be rendered
		Pipeline::IA::bindIndexBuffer(device_context, index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
		Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}


	// Draw the mesh with the given start index and index count
	void draw(ID3D11DeviceContext& device_context, u32 index_count, u32 start_index) {
		device_context.DrawIndexed(index_count, start_index, 0);
	}


public:
	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;

	u32 vertex_count;
	u32 index_count;
	u32 stride;
};


#include "mesh.tpp"
