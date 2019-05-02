
namespace render {

template<typename VertexT>
Mesh::Mesh(ID3D11Device& device,
           const std::string& name,
	       const std::vector<VertexT>& vertices,
	       const std::vector<u32>& indices)
	: name(name)
{
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

} //namespace render