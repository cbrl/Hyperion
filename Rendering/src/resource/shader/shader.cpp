#include "shader.h"


namespace render {

VertexShader::VertexShader(const std::wstring& guid,
                           ID3D11Device& device,
                           const ShaderBytecode& bytecode,
                           std::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
	: Resource(guid) {

	createShader(device, bytecode, input_element_descs);
}


void VertexShader::createShader(ID3D11Device& device,
                                const ShaderBytecode& bytecode,
                                std::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs) {

	// Create the shader
	ThrowIfFailed(device.CreateVertexShader(bytecode.getBytecode(),
											bytecode.getBytecodeSize(),
											nullptr,
											shader.GetAddressOf()),
				  "Failed to create vertex shader");


	// Create the input layout, if specified.
	if (input_element_descs.data() == nullptr) return;
	ThrowIfFailed(device.CreateInputLayout(input_element_descs.data(),
										   static_cast<UINT>(input_element_descs.size()),
										   bytecode.getBytecode(),
										   bytecode.getBytecodeSize(),
										   layout.ReleaseAndGetAddressOf()),
				  "Failed to create input layout");
}

} //namespace render