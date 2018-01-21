#include "stdafx.h"
#include "Model.h"


Model::Model() : m_VertexBuffer(nullptr){

}


Model::~Model() {
}


bool Model::Init(const ComPtr<ID3D11Device>& device, const char* modelFilename,
				 ComPtr<ID3D11ShaderResourceView> texture, ShaderTypes shader) {

	if (!LoadModel(modelFilename)) {
		return false;
	}

	if (!InitBuffers(device)) {
		return false;
	}

	m_Texture = texture;
	m_Shader  = shader;

	return true;
}


bool Model::LoadModel(const char* filename) {
	char      ch;
	ifstream  file;
	ModelData temp;

	file.open(filename);
	if (file.fail()) return false;

	// Move file pointer to vertex count
	file.get(ch);
	while (ch != ':') {
		file.get(ch);
	}

	// Read vertex count, copy value to index count
	file >> m_VertexCount;
	m_IndexCount = m_VertexCount;

	// Read up to vertex data
	file.get(ch);
	while (ch != ':') {
		file.get(ch);
	}
	file.get(ch);
	file.get(ch);

	// Store vertex data
	for (int i = 0; i < m_VertexCount; i++) {
		file >> temp.x >> temp.y >> temp.z;
		file >> temp.tu >> temp.tv;
		file >> temp.nx >> temp.ny >> temp.nz;

		m_ModelData.push_back(temp);
	}
	
	file.close();

	return true;
}


bool Model::InitBuffers(const ComPtr<ID3D11Device>& device) {
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vector<ULONG>                       indices;
	vector<VertexPositionNormalTexture> vertices;
	VertexPositionNormalTexture         temp;


	// Fill vertex and index vector data
	for (int i = 0; i < m_VertexCount; i++) {
		temp.position           = XMFLOAT3(m_ModelData[i].x, m_ModelData[i].y, m_ModelData[i].z);
		temp.textureCoordinate  = XMFLOAT2(m_ModelData[i].tu, m_ModelData[i].tv);
		temp.normal             = XMFLOAT3(m_ModelData[i].nx, m_ModelData[i].ny, m_ModelData[i].nz);
		//temp.color = XMFLOAT4((float)i / m_VertexCount, 1.0f - ((float)i / m_VertexCount), 1.0f, 1.0f);

		vertices.push_back(temp);
		indices.push_back(i);
	}

	// Vertex m_Buffer description
	vertexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth      = sizeof(VertexPositionNormalTexture) * m_VertexCount;
	vertexBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags      = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem          = const_cast<VertexPositionNormalTexture*>(vertices.data());
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex m_Buffer
	HR(device->CreateBuffer(&vertexBufferDesc, &vertexData, m_VertexBuffer.GetAddressOf()));


	// Index m_Buffer description
	indexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth      = sizeof(ULONG) * m_IndexCount;
	indexBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags      = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = const_cast<ULONG*>(indices.data());
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index m_Buffer
	HR(device->CreateBuffer(&indexBufferDesc, &indexData, m_IndexBuffer.GetAddressOf()));

	return true;
}


void Model::RenderBuffers(const ComPtr<ID3D11DeviceContext>& deviceContext) {
	UINT stride, offset;

	stride = sizeof(VertexPositionNormalTexture);
	offset = 0;

	// Set vertex m_Buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// Set index m_Buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set type of primitive that should be rendered from this vertex m_Buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}