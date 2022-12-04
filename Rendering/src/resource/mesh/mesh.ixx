module;

#include <typeinfo>
#include <typeindex>

#include "datatypes/scalar_types.h"

#include "directx/d3d11.h"

export module rendering:mesh;

import :pipeline;


namespace render {

export class Mesh final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	template<typename VertexT>
	Mesh(ID3D11Device& device,
	     const std::string& name,
	     const std::vector<VertexT>& vertices,
	     const std::vector<u32>& indices)
		: name(name)
		, vertex_type(typeid(VertexT)) {

		vertex_count = static_cast<u32>(vertices.size());
		index_count  = static_cast<u32>(indices.size());
		stride       = sizeof(VertexT);

		D3D11_BUFFER_DESC vb_desc = {};
		D3D11_BUFFER_DESC ib_desc = {};

		D3D11_SUBRESOURCE_DATA vb_data = {};
		D3D11_SUBRESOURCE_DATA ib_data = {};


		// Vertex buffer description
		vb_desc.Usage               = D3D11_USAGE_DEFAULT;
		vb_desc.ByteWidth           = sizeof(VertexT) * vertex_count;
		vb_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		vb_desc.CPUAccessFlags      = 0;
		vb_desc.MiscFlags           = 0;
		vb_desc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data
		vb_data.pSysMem          = const_cast<VertexT*>(vertices.data());
		vb_data.SysMemPitch      = 0;
		vb_data.SysMemSlicePitch = 0;

		// Create vertex buffer
		ThrowIfFailed(device.CreateBuffer(&vb_desc, &vb_data, vertex_buffer.GetAddressOf()),
					  "Failed to create mesh vertex buffer");


		// Index buffer description
		ib_desc.Usage               = D3D11_USAGE_DEFAULT;
		ib_desc.ByteWidth           = sizeof(u32) * index_count;
		ib_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		ib_desc.CPUAccessFlags      = 0;
		ib_desc.MiscFlags           = 0;
		ib_desc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data
		ib_data.pSysMem          = const_cast<u32*>(indices.data());
		ib_data.SysMemPitch      = 0;
		ib_data.SysMemSlicePitch = 0;

		// Create index buffer
		ThrowIfFailed(device.CreateBuffer(&ib_desc, &ib_data, index_buffer.GetAddressOf()),
					  "Failed to create mesh index buffer");
	}

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
		// Set vertex buffer to active in the input assembler so it can be rendered
		Pipeline::IA::bindVertexBuffer(device_context, 0, vertex_buffer.Get(), stride, 0);

		// Set index buffer to active in the input assembler so it can be rendered
		Pipeline::IA::bindIndexBuffer(device_context, index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
		Pipeline::IA::bindPrimitiveTopology(device_context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	[[nodiscard]]
	const std::string& getName() const noexcept {
		return name;
	}

	[[nodiscard]]
	u32 getVertexCount() const noexcept {
		return vertex_count;
	}

	[[nodiscard]]
	u32 getIndexCount() const noexcept {
		return index_count;
	}

	[[nodiscard]]
	ID3D11Buffer* getVertexBuffer() const noexcept {
		return vertex_buffer.Get();
	}

	[[nodiscard]]
	ID3D11Buffer* getIndexBuffer() const noexcept {
		return index_buffer.Get();
	}

	[[nodiscard]]
	const std::type_index& getVertexType() const noexcept {
		return vertex_type;
	}


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	std::string name;

	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;

	u32 vertex_count;
	u32 index_count;
	u32 stride;

	// The vertex type used in the mesh
	std::type_index vertex_type;
};


} //namespace render
