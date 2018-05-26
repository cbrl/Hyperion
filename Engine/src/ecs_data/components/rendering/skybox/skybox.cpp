#include "stdafx.h"
#include "skybox.h"


void SkyBox::Init(ID3D11Device& device,
				  shared_ptr<Texture> tex) {

	// Create the texture
	texture = std::move(tex);

	// Set the vertex stride
	stride = sizeof(VertexPosition);


	// Create a sphere
	vector<VertexPosition> vertices;
	vector<u32> indices;

	Shapes::ComputeSphere(vertices, indices, 5000.0f, 12, true, true);


	// Create the vertex buffer
	D3D11_BUFFER_DESC vb_desc;
	vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vb_desc.ByteWidth = static_cast<u32>(sizeof(float3) * vertices.size());
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = 0;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vb_data;
	vb_data.pSysMem = &vertices[0];

	device.CreateBuffer(&vb_desc, &vb_data, vertex_buffer.GetAddressOf());


	// Create the index buffer
	index_count = static_cast<u32>(indices.size());

	D3D11_BUFFER_DESC ib_desc;
	ib_desc.Usage = D3D11_USAGE_IMMUTABLE;
	ib_desc.ByteWidth = sizeof(u32) * index_count;
	ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ib_desc.CPUAccessFlags = 0;
	ib_desc.StructureByteStride = 0;
	ib_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ib_data;
	ib_data.pSysMem = &indices[0];

	device.CreateBuffer(&ib_desc, &ib_data, index_buffer.GetAddressOf());
}
