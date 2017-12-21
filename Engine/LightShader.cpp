#include "stdafx.h"
#include "LightShader.h"


LightShader::LightShader() {
}


LightShader::~LightShader() {
}


void LightShader::CreatePolygonLayout() {
	D3D11_INPUT_ELEMENT_DESC temp;

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	temp.SemanticName = "POSITION";
	temp.SemanticIndex = 0;
	temp.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	temp.InputSlot = 0;
	temp.AlignedByteOffset = 0;
	temp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	temp.InstanceDataStepRate = 0;
	m_PolygonLayout.push_back(temp);

	temp.SemanticName = "TEXCOORD";
	temp.SemanticIndex = 0;
	temp.Format = DXGI_FORMAT_R32G32_FLOAT;
	temp.InputSlot = 0;
	temp.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	temp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	temp.InstanceDataStepRate = 0;
	m_PolygonLayout.push_back(temp);

	temp.SemanticName = "NORMAL";
	temp.SemanticIndex = 0;
	temp.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	temp.InputSlot = 0;
	temp.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	temp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	temp.InstanceDataStepRate = 0;
	m_PolygonLayout.push_back(temp);
}


bool LightShader::InitBuffers(ComPtr<ID3D11Device>& device) {
	// Setup the description of the camera dynamic constant buffer that is in the vertex shader
	m_CameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_CameraBufferDesc.ByteWidth = sizeof(CameraBuffer);
	m_CameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_CameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_CameraBufferDesc.MiscFlags = 0;
	m_CameraBufferDesc.StructureByteStride = 0;


	// Setup the description of the light dynamic constant buffer that is in the pixel shader
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail
	m_LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_LightBufferDesc.ByteWidth = sizeof(LightBuffer);
	m_LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_LightBufferDesc.MiscFlags = 0;
	m_LightBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HR(device->CreateBuffer(&m_CameraBufferDesc, nullptr, m_CameraBuffer.ReleaseAndGetAddressOf()));

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	HR(device->CreateBuffer(&m_LightBufferDesc, nullptr, m_LightBuffer.ReleaseAndGetAddressOf()));

	return true;
}


bool LightShader::Render(ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
						 XMFLOAT3 cameraPosition, ComPtr<ID3D11ShaderResourceView> texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor,
						 XMFLOAT4 diffuseColor, XMFLOAT4 specularColor, float specularPower) {
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, cameraPosition, texture, lightDirection,
						ambientColor, diffuseColor, specularColor, specularPower);
	if (!result) return false;

	RenderShader(deviceContext, indexCount);

	return true;
}


bool LightShader::SetShaderParameters(ComPtr<ID3D11DeviceContext>& deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
									  XMFLOAT3 cameraPosition, ComPtr<ID3D11ShaderResourceView> texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor,
									  XMFLOAT4 diffuseColor, XMFLOAT4 specularColor, float specularPower) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	UINT bufferNumber;
	MatrixBuffer *matrixPtr;
	LightBuffer  *lightPtr;
	CameraBuffer *cameraPtr;

	// Transpose the matrices to prepare them for the shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);



	// Lock the constant buffer so it can be written to
	HR(deviceContext->Map(m_MatrixBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource));

	// Get a pointer to the data in the constant buffer
	matrixPtr = (MatrixBuffer*)mappedResource.pData;

	// Copy the matrices into the constant buffer
	matrixPtr->world = worldMatrix;
	matrixPtr->view = viewMatrix;
	matrixPtr->projection = projectionMatrix;

	// Unlock the constant buffer
	deviceContext->Unmap(m_MatrixBuffer.Get(), NULL);

	// Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_MatrixBuffer.GetAddressOf());

	// Set shader texture resource in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// Lock the light constant buffer so it can be written to
	HR(deviceContext->Map(m_LightBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource));



	// Get a pointer to the data in the constant buffer
	lightPtr = (LightBuffer*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer
	lightPtr->ambientColor = ambientColor;
	lightPtr->diffuseColor = diffuseColor;
	lightPtr->lightDirection = lightDirection;
	lightPtr->specularColor = specularColor;
	lightPtr->specularPower = specularPower;

	// Unlock the constant buffer
	deviceContext->Unmap(m_LightBuffer.Get(), NULL);

	// Set the position of the light constant buffer in the pixel shader
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_LightBuffer.GetAddressOf());



	// Lock the camera constant buffer so it can be written to
	HR(deviceContext->Map(m_CameraBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource));

	// Get a pointer to the data in the constant buffer
	cameraPtr = (CameraBuffer*)mappedResource.pData;

	// Copy the camera position into the constant buffer
	cameraPtr->cameraPosition = cameraPosition;
	cameraPtr->padding = 0.0f;

	// Unlock the camera constant buffer
	deviceContext->Unmap(m_CameraBuffer.Get(), NULL);

	// Set the position of the camera constant buffer in the vertex shader
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_CameraBuffer.GetAddressOf());


	return true;
}


void LightShader::RenderShader(ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount) {
	// Set the vertex input layout
	deviceContext->IASetInputLayout(m_Layout.Get());

	// Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	// Set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());

	// Render
	deviceContext->DrawIndexed(indexCount, 0, 0);
}