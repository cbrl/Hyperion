#include "shader.h"

#include "util/engine_util.h"
#include "io/io.h"


//----------------------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------------------

VertexShader::VertexShader(ID3D11Device& device,
                           const wchar_t* filename,
                           const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
                           size_t numElements) {
	HRESULT result;
	ComPtr<ID3D10Blob> error_message;
	ComPtr<ID3D10Blob> shader_buffer;

	// Shader compile flags
	u32 flags = D3D10_SHADER_ENABLE_STRICTNESS;

	// Disable optimization in debug builds
	#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	// Compile the vertex shader
	result = D3DCompileFromFile(filename,
	                            nullptr,
	                            D3D_COMPILE_STANDARD_FILE_INCLUDE,
	                            "VS",
	                            "vs_5_0",
	                            flags,
	                            NULL,
	                            shader_buffer.GetAddressOf(),
	                            error_message.GetAddressOf());

	if (FAILED(result)) {
		if (error_message) {
			OutputShaderErrorMessage(error_message.Get(), filename);
		}
		else {
			MessageBox(nullptr, filename, L"Missing vertex shader file", MB_OK);
		}

		ThrowIfFailed(result, "Error compiling vertex shader");
	}


	// Create the vertex shader
	ThrowIfFailed(device.CreateVertexShader(shader_buffer->GetBufferPointer(),
	                                        shader_buffer->GetBufferSize(),
	                                        nullptr,
	                                        shader.ReleaseAndGetAddressOf()),
	              "Failed to create vertex shader");


	// Create the vertex input layout
	ThrowIfFailed(device.CreateInputLayout(inputElementDesc,
	                                       static_cast<UINT>(numElements),
	                                       shader_buffer->GetBufferPointer(),
	                                       shader_buffer->GetBufferSize(),
	                                       layout.ReleaseAndGetAddressOf()),
	              "Failed to create input layout");
}


VertexShader::VertexShader(ID3D11Device& device,
                           const BYTE* buffer,
                           size_t size,
                           const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
                           size_t numElements) {

	// Create the shader
	ThrowIfFailed(device.CreateVertexShader(buffer, size, nullptr, shader.ReleaseAndGetAddressOf()),
	              "Failed to create vertex shader");

	// Create the input layout
	ThrowIfFailed(device.CreateInputLayout(inputElementDesc,
	                                       static_cast<UINT>(numElements),
	                                       buffer,
	                                       size,
	                                       layout.ReleaseAndGetAddressOf()),
	              "Failed to create input layout");
}


//----------------------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------------------

PixelShader::PixelShader(ID3D11Device& device, const wchar_t* filename) {
	HRESULT result;
	ComPtr<ID3D10Blob> error_message;
	ComPtr<ID3D10Blob> shader_buffer;

	// Shader compile flags
	u32 flags = D3D10_SHADER_ENABLE_STRICTNESS;

	// Disable optimization in debug builds
	#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	// Compile the pixel shader
	result = D3DCompileFromFile(filename,
	                            nullptr,
	                            D3D_COMPILE_STANDARD_FILE_INCLUDE,
	                            "PS",
	                            "ps_5_0",
	                            flags,
	                            NULL,
	                            shader_buffer.GetAddressOf(),
	                            error_message.GetAddressOf());

	if (FAILED(result)) {
		if (error_message) {
			OutputShaderErrorMessage(error_message.Get(), filename);
		}
		else {
			MessageBox(nullptr, filename, L"Missing pixel shader file", MB_OK);
		}

		ThrowIfFailed(result, "Error compiling pixel shader");
	}


	// Create the pixel shader
	ThrowIfFailed(device.CreatePixelShader(shader_buffer->GetBufferPointer(),
	                                       shader_buffer->GetBufferSize(),
	                                       nullptr,
	                                       shader.ReleaseAndGetAddressOf()),
	              "Failed to create pixel shader");
}


PixelShader::PixelShader(ID3D11Device& device,
                         const BYTE* buffer,
                         size_t size) {

	ThrowIfFailed(device.CreatePixelShader(buffer, size, nullptr, shader.ReleaseAndGetAddressOf()),
	              "Failed to create vertex shader");
}


//----------------------------------------------------------------------------------
// Display error message if shader compilation failed
//----------------------------------------------------------------------------------

void OutputShaderErrorMessage(ID3D10Blob* error_message, const WCHAR* shader_filename) {
	ofstream fout;

	// Get a pointer to the error message text buffer
	const auto compile_errors = static_cast<char*>(error_message->GetBufferPointer());

	// Get the length of the message
	const size_t buffer_size = error_message->GetBufferSize();

	fout.open("shader_error.txt");

	// Write out the error message
	for (size_t i = 0; i < buffer_size; i++) {
		fout << compile_errors[i];
	}

	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for compile errors
	MessageBox(nullptr, L"Error compiling shader.  Check shader_error.txt", shader_filename, MB_OK);
}
