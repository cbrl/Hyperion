#include "shader.h"


VertexShader::VertexShader(const std::wstring& guid,
                           ID3D11Device& device,
                           const ShaderBytecode& bytecode,
                           const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
                           size_t numElements)
	: Resource(guid) {

	createShader(device, bytecode, inputElementDesc, numElements);
}


void VertexShader::createShader(ID3D11Device& device,
                           const ShaderBytecode& bytecode,
                           const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
                           size_t numElements) {

	// Create the shader
	ThrowIfFailed(device.CreateVertexShader(bytecode.getBytecode(),
											bytecode.getBytecodeSize(),
											nullptr,
											shader.GetAddressOf()),
				  "Failed to create vertex shader");


	// Create the input layout, if specified.
	if (!inputElementDesc) return;
	ThrowIfFailed(device.CreateInputLayout(inputElementDesc,
										   static_cast<UINT>(numElements),
										   bytecode.getBytecode(),
										   bytecode.getBytecodeSize(),
										   layout.ReleaseAndGetAddressOf()),
				  "Failed to create input layout");
}