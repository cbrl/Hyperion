#include "stdafx.h"
#include "Shader.h"


Shader::Shader() {
}


Shader::~Shader() {
}


void Shader::Init(HWND hWnd, ID3D11Device* device, const WCHAR* vsFilename, const WCHAR* psFilename,
                  const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, size_t numElements) {
	HRESULT result;
	ID3D10Blob* errorMessage = {};
	ID3D10Blob* vertexShaderBuffer = {};
	ID3D10Blob* pixelShaderBuffer = {};

	// Compile vertex shader
	result = D3DCompileFromFile(vsFilename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
								D3D10_SHADER_ENABLE_STRICTNESS, NULL, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		if (errorMessage) {
			OutputShaderErrorMessage(hWnd, errorMessage, vsFilename);
		}
		else {
			MessageBox(hWnd, vsFilename, L"Missing vertex shader file", MB_OK);
		}

		DX::ThrowIfFailed(result, "Error compiling vertex shader");
	}

	// Compile pixel shader
	result = D3DCompileFromFile(psFilename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
								D3D10_SHADER_ENABLE_STRICTNESS, NULL, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result)) {
		if (errorMessage) {
			OutputShaderErrorMessage(hWnd, errorMessage, psFilename);
		}
		else {
			MessageBox(hWnd, psFilename, L"Missing pixel shader file", MB_OK);
		}

		DX::ThrowIfFailed(result, "Error compiling pixel shader");
	}


	// Create vertex and pixel shader
	DX::ThrowIfFailed(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
	                                             nullptr, vertex_shader.ReleaseAndGetAddressOf()),
	                  "Failed to create vertex shader");

	DX::ThrowIfFailed(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
	                             nullptr, pixel_shader.ReleaseAndGetAddressOf()),
	                  "Failed to create pixel shader");


	// Create the vertex input layout
	DX::ThrowIfFailed(device->CreateInputLayout(inputElementDesc, numElements, vertexShaderBuffer->GetBufferPointer(),
								                vertexShaderBuffer->GetBufferSize(), vertex_layout.ReleaseAndGetAddressOf()),
	                  "Failed to create input layout");
}


void Shader::BindShader(ID3D11DeviceContext* device_context) {
	device_context->IASetInputLayout(vertex_layout.Get());

	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
}


void Shader::OutputShaderErrorMessage(HWND hWnd, ID3D10Blob* errorMessage, const WCHAR* shaderFilename) {
	char*    compileErrors;
	size_t   bufferSize;
	ofstream fout;


	// Get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader_error.txt");

	// Write out the error message
	for (size_t i = 0; i < bufferSize; i++) {
		fout << compileErrors[i];
	}

	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for compile errors
	MessageBox(hWnd, L"Error compiling shader.  Check shader_error.txt", shaderFilename, MB_OK);
}