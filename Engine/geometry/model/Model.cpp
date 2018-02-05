#include "stdafx.h"
#include "Model.h"


Model::Model() :
	position(XMMatrixTranslation(0.0f, 0.0f, 0.0f)),
	rotation(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f))
{
}


Model::~Model() {
}


HRESULT Model::Init(ID3D11Device* device, const char* modelFilename,
				 ComPtr<ID3D11ShaderResourceView> modelTexture, ShaderTypes shaderType) {
	HRESULT hr;

	hr = LoadModel(modelFilename);
	if (FAILED(hr)) {
		return hr;
	}

	hr = InitBuffers(device);
	if (FAILED(hr)) {
		return hr;
	}

	texture = modelTexture;
	shader  = shaderType;

	return S_OK;
}


HRESULT Model::LoadModel(const char* filename) {
	char      ch;
	ifstream  file;
	ModelData temp;

	file.open(filename);
	if (file.fail()) return E_FAIL;

	// Move file pointer to vertex count
	file.get(ch);
	while (ch != ':') {
		file.get(ch);
	}

	// Read vertex count, copy value to index count
	file >> vertexCount;
	indexCount = vertexCount;

	// Read up to vertex data
	file.get(ch);
	while (ch != ':') {
		file.get(ch);
	}
	file.get(ch);
	file.get(ch);

	// Store vertex data
	for (int i = 0; i < vertexCount; i++) {
		file >> temp.x >> temp.y >> temp.z;
		file >> temp.tu >> temp.tv;
		file >> temp.nx >> temp.ny >> temp.nz;

		modelData.push_back(temp);
	}
	
	file.close();

	return S_OK;
}


HRESULT Model::InitBuffers(ID3D11Device* device) {
	HRESULT hr;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vector<ULONG>                       indices;
	vector<VertexPositionNormalTexture> vertices;
	VertexPositionNormalTexture         temp;


	// Fill vertex and index vector data
	for (int i = 0; i < vertexCount; i++) {
		temp.position           = XMFLOAT3(modelData[i].x, modelData[i].y, modelData[i].z);
		temp.textureCoordinate  = XMFLOAT2(modelData[i].tu, modelData[i].tv);
		temp.normal             = XMFLOAT3(modelData[i].nx, modelData[i].ny, modelData[i].nz);
		//temp.color = XMFLOAT4((float)i / vertexCount, 1.0f - ((float)i / vertexCount), 1.0f, 1.0f);

		vertices.push_back(temp);
		indices.push_back(i);
	}

	// Vertex buffer description
	vertexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth      = sizeof(VertexPositionNormalTexture) * vertexCount;
	vertexBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags      = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem          = const_cast<VertexPositionNormalTexture*>(vertices.data());
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer.GetAddressOf());

	// Index buffer description
	indexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth      = sizeof(ULONG) * indexCount;
	indexBufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags      = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = const_cast<ULONG*>(indices.data());
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	hr = device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf());

	return hr;
}


void Model::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	UINT stride, offset;

	stride = sizeof(VertexPositionNormalTexture);
	offset = 0;

	// Set vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set type of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}