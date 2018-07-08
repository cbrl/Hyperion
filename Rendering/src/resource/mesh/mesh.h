#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"


class Mesh final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	template<typename VertexT>
	Mesh(ID3D11Device& device, const vector<VertexT>& vertices, const vector<u32>& indices);

	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Mesh() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Mesh& operator=(const Mesh& mesh) = delete;
	Mesh& operator=(Mesh&& mesh) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

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


public:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;

	u32 vertex_count;
	u32 index_count;
	u32 stride;
};


#include "mesh.tpp"
