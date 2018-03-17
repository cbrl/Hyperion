#include "stdafx.h"
#include "shader.h"


//----------------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------------

VertexShader::VertexShader(ID3D11Device* device,
						   const WCHAR* filename,
						   const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
						   size_t numElements) {
	HRESULT result;
	ComPtr<ID3D10Blob> error_message = {};
	ComPtr<ID3D10Blob> shader_buffer = {};

	// Shader compile flags
	u32 flags = D3D10_SHADER_ENABLE_STRICTNESS; 

	// Disable optimization in debug builds
	#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	// Compile the vertex shader
	result = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0",
								flags, NULL, shader_buffer.GetAddressOf(), error_message.GetAddressOf());
	if (FAILED(result)) {
		if (error_message) {
			OutputShaderErrorMessage(error_message.Get(), filename);
		}
		else {
			MessageBox(NULL, filename, L"Missing vertex shader file", MB_OK);
		}

		ThrowIfFailed(result, "Error compiling vertex shader");
	}


	// Create the vertex shader
	ThrowIfFailed(device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(),
											 nullptr, shader.ReleaseAndGetAddressOf()),
				  "Failed to create vertex shader");


	// Create the vertex input layout
	ThrowIfFailed(device->CreateInputLayout(inputElementDesc, static_cast<u32>(numElements), shader_buffer->GetBufferPointer(),
											shader_buffer->GetBufferSize(), layout.ReleaseAndGetAddressOf()),
				  "Failed to create input layout");
}



//----------------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------------

PixelShader::PixelShader(ID3D11Device* device, const WCHAR* filename) {
	HRESULT result;
	ComPtr<ID3D10Blob> error_message = {};
	ComPtr<ID3D10Blob> shader_buffer = {};

	// Shader compile flags
	u32 flags = D3D10_SHADER_ENABLE_STRICTNESS;

	// Disable optimization in debug builds
	#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	// Compile the pixel shader
	result = D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0",
								flags, NULL, shader_buffer.GetAddressOf(), error_message.GetAddressOf());
	if (FAILED(result)) {
		if (error_message) {
			OutputShaderErrorMessage(error_message.Get(), filename);
		}
		else {
			MessageBox(NULL, filename, L"Missing pixel shader file", MB_OK);
		}

		ThrowIfFailed(result, "Error compiling pixel shader");
	}


	// Create the pixel shader
	ThrowIfFailed(device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(),
												nullptr, shader.ReleaseAndGetAddressOf()),
				  "Failed to create pixel shader");
}



//----------------------------------------------------------------------------------
// Display error message if shader compilation failed
//----------------------------------------------------------------------------------

void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* shaderFilename) {
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
	MessageBox(NULL, L"Error compiling shader.  Check shader_error.txt", shaderFilename, MB_OK);
}