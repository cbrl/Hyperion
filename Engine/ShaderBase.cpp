#include "stdafx.h"
#include "ShaderBase.h"


ShaderBase::ShaderBase() {
}


ShaderBase::~ShaderBase() {
}


bool ShaderBase::Init(const ComPtr<ID3D11Device>& device, HWND hWnd, const WCHAR* vsFilename, const WCHAR* psFilename) {
	CreatePolygonLayout();

	if (!InitShader(device, hWnd, vsFilename, psFilename)) {
		return false;
	}

	InitBuffers(device);

	return true;
}


bool ShaderBase::InitShader(const ComPtr<ID3D11Device>& device, HWND hWnd, const WCHAR* vsFilename, const WCHAR* psFilename) {
	HRESULT result;
	ComPtr<ID3D10Blob> errorMessage;
	ComPtr<ID3D10Blob> vertexShaderBuffer;
	ComPtr<ID3D10Blob> pixelShaderBuffer;
	UINT numElements;


	// Compile vertex shader
	result = D3DCompileFromFile(vsFilename, nullptr, nullptr, m_VSEntrypoint.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL,
								vertexShaderBuffer.ReleaseAndGetAddressOf(), errorMessage.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage, hWnd, vsFilename);
		}
		else {
			MessageBox(hWnd, vsFilename, L"Missing vertex shader file", MB_OK);
		}

		return false;
	}

	// Compile pixel shader
	result = D3DCompileFromFile(psFilename, nullptr, nullptr, m_PSEntrypoint.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, NULL,
								pixelShaderBuffer.ReleaseAndGetAddressOf(), errorMessage.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage, hWnd, psFilename);
		}
		else {
			MessageBox(hWnd, psFilename, L"Missing pixel shader file", MB_OK);
		}

		return false;
	}
	
	// Create vertex and pixel shader
	HR(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, m_VertexShader.ReleaseAndGetAddressOf()));
	HR(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, m_PixelShader.ReleaseAndGetAddressOf()));


	// Get the number of elements in the polygon layout
	numElements = m_PolygonLayout.size();

	// Create the vertex input layout
	HR(device->CreateInputLayout(m_PolygonLayout.data(), numElements, vertexShaderBuffer->GetBufferPointer(),
								 vertexShaderBuffer->GetBufferSize(), m_Layout.ReleaseAndGetAddressOf()));


	// Create a texture sampler state description
	m_SamplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_SamplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.MipLODBias     = 0.0f;
	m_SamplerDesc.MaxAnisotropy  = 1;
	m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	m_SamplerDesc.BorderColor[0] = 0;
	m_SamplerDesc.BorderColor[1] = 0;
	m_SamplerDesc.BorderColor[2] = 0;
	m_SamplerDesc.BorderColor[3] = 0;
	m_SamplerDesc.MinLOD         = 0;
	m_SamplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	// Create the texture sampler state
	HR(device->CreateSamplerState(&m_SamplerDesc, m_SamplerState.ReleaseAndGetAddressOf()));


	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader
	m_MatrixBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
	m_MatrixBufferDesc.ByteWidth      = sizeof(MatrixBuffer);
	m_MatrixBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	m_MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_MatrixBufferDesc.MiscFlags      = 0;
	m_MatrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	HR(device->CreateBuffer(&m_MatrixBufferDesc, nullptr, m_MatrixBuffer.ReleaseAndGetAddressOf()));


	return true;
}


void ShaderBase::OutputShaderErrorMessage(ComPtr<ID3D10Blob> errorMessage, HWND hWnd, const WCHAR* shaderFilename) {
	char* compileErrors;
	ULONG bufferSize;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader_error.txt");

	// Write out the error message.
	for (ULONG i = 0; i < bufferSize; i++) {
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hWnd, L"Error compiling shader.  Check shader_error.txt", shaderFilename, MB_OK);
}
