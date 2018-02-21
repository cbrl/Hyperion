#include "stdafx.h"
#include "shader.h"


//----------------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------------

void VertexShader::Init(HWND hWnd, ID3D11Device* device, const WCHAR* filename,
						const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, size_t numElements) {
	HRESULT result;
	ID3D10Blob* error_message = {};
	ID3D10Blob* shader_buffer = {};

	// Compile vertex shader
	result = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
								D3D10_SHADER_ENABLE_STRICTNESS, NULL, &shader_buffer, &error_message);
	if (FAILED(result)) {
		if (error_message) {
			ShaderError::OutputShaderErrorMessage(hWnd, error_message, filename);
		}
		else {
			MessageBox(hWnd, filename, L"Missing vertex shader file", MB_OK);
		}

		DX::ThrowIfFailed(result, "Error compiling vertex shader");
	}


	// Create the vertex shader
	DX::ThrowIfFailed(device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(),
												 nullptr, shader.ReleaseAndGetAddressOf()),
					  "Failed to create vertex shader");


	// Create the vertex input layout
	DX::ThrowIfFailed(device->CreateInputLayout(inputElementDesc, static_cast<uint32_t>(numElements), shader_buffer->GetBufferPointer(),
												shader_buffer->GetBufferSize(), layout.ReleaseAndGetAddressOf()),
					  "Failed to create input layout");
}



//----------------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------------

void PixelShader::Init(HWND hWnd, ID3D11Device* device, const WCHAR* filename) {
	HRESULT result;
	ID3D10Blob* error_message = {};
	ID3D10Blob* shader_buffer = {};


	// Compile the pixel shader
	result = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
								D3D10_SHADER_ENABLE_STRICTNESS, NULL, &shader_buffer, &error_message);
	if (FAILED(result)) {
		if (error_message) {
			ShaderError::OutputShaderErrorMessage(hWnd, error_message, filename);
		}
		else {
			MessageBox(hWnd, filename, L"Missing pixel shader file", MB_OK);
		}

		DX::ThrowIfFailed(result, "Error compiling pixel shader");
	}


	// Create the pixel shader
	DX::ThrowIfFailed(device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(),
												nullptr, shader.ReleaseAndGetAddressOf()),
					  "Failed to create pixel shader");
}



//----------------------------------------------------------------------------------
// Display error message if shader compilation failed
//----------------------------------------------------------------------------------

void ShaderError::OutputShaderErrorMessage(HWND hWnd, ID3D10Blob* errorMessage, const WCHAR* shaderFilename) {
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