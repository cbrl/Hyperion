#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"


class Mesh {
	public:
		Mesh() = default;
		~Mesh() = default;

		template<typename VertexT>
		Mesh(ID3D11Device* device, const vector<VertexT>& vertices, const vector<u32>& indices);


		// Draw the mesh with the given start index and index count
		void Draw(ID3D11DeviceContext* device_context, u32 index_count, u32 start_index) const {
			u32 offset = 0;

			// Set vertex buffer to active in the input assembler so it can be rendered
			Pipeline::IA::BindVertexBuffers(device_context, 0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
			
			// Set index buffer to active in the input assembler so it can be rendered
			Pipeline::IA::BindIndexBuffer(device_context, index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			
			// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
			Pipeline::IA::BindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			device_context->DrawIndexed(index_count, start_index, 0);
		}


	public:
		ComPtr<ID3D11Buffer> vertex_buffer;
		ComPtr<ID3D11Buffer> index_buffer;

		u32 vertex_count;
		u32 index_count;
		u32 stride;
};


#include "mesh.tpp"