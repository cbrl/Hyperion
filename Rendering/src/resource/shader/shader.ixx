module;

#include "directx/d3d11.h"

export module rendering:shader;

import exception;
import :resource;
import :shader_bytecode;
import :pipeline;


export namespace render {

template<typename ShaderT, typename StageT>
class Shader final : public Resource<Shader<ShaderT, StageT>> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Shader(const std::wstring& guid,
		   ID3D11Device& device,
		   const ShaderBytecode& bytecode)
		: Resource<Shader>(guid) {

		createShader(device, bytecode);
	}

	Shader(const Shader& shader) = delete;
	Shader(Shader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Shader() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Shader& operator=(const Shader& shader) = delete;
	Shader& operator=(Shader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Bind the vertex shader to a pipeline stage
	void bind(ID3D11DeviceContext& device_context) const {
		StageT::bindShader(device_context, shader.Get(), {});
	}

private:

	void createShader(ID3D11Device& device, const ShaderBytecode& bytecode);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ComPtr<ShaderT> shader;
};


template<>
class Shader<ID3D11VertexShader, Pipeline::VS> final : public Resource<Shader<ID3D11VertexShader, Pipeline::VS>> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	Shader(const std::wstring& guid,
		ID3D11Device& device,
		const ShaderBytecode& bytecode,
		std::span<const D3D11_INPUT_ELEMENT_DESC> input_element_descs)
		: Resource(guid) {

		createShader(device, bytecode, input_element_descs);
	}

	Shader(const Shader& shader) = delete;
	Shader(Shader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~Shader() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	Shader& operator=(const Shader& shader) = delete;
	Shader& operator=(Shader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Bind the vertex shader to a pipeline stage
	void bind(ID3D11DeviceContext& device_context) const {
		if (layout) Pipeline::IA::bindInputLayout(device_context, layout.Get());
		Pipeline::VS::bindShader(device_context, shader.Get(), {});
	}

private:

	void createShader(ID3D11Device& device,
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


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ComPtr<ID3D11VertexShader> shader;
	ComPtr<ID3D11InputLayout>  layout;
};


using VertexShader   = Shader<ID3D11VertexShader, Pipeline::VS>;
using ComputeShader  = Shader<ID3D11ComputeShader, Pipeline::CS>;
using DomainShader   = Shader<ID3D11DomainShader, Pipeline::DS>;
using GeometryShader = Shader<ID3D11GeometryShader, Pipeline::GS>;
using HullShader     = Shader<ID3D11HullShader, Pipeline::HS>;
using PixelShader    = Shader<ID3D11PixelShader, Pipeline::PS>;


template<>
void ComputeShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {
	ThrowIfFailed(device.CreateComputeShader(bytecode.getBytecode(),
		bytecode.getBytecodeSize(),
		nullptr,
		shader.GetAddressOf()),
		"Failed to create compute shader");
}

template<>
void DomainShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {
	ThrowIfFailed(device.CreateDomainShader(bytecode.getBytecode(),
		bytecode.getBytecodeSize(),
		nullptr,
		shader.GetAddressOf()),
		"Failed to create domain shader");
}

template<>
void GeometryShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {
	ThrowIfFailed(device.CreateGeometryShader(bytecode.getBytecode(),
		bytecode.getBytecodeSize(),
		nullptr,
		shader.GetAddressOf()),
		"Failed to create geometry shader");
}

template<>
void HullShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {
	ThrowIfFailed(device.CreateHullShader(bytecode.getBytecode(),
		bytecode.getBytecodeSize(),
		nullptr,
		shader.GetAddressOf()),
		"Failed to create hull shader");
}

template<>
void PixelShader::createShader(ID3D11Device& device, const ShaderBytecode& bytecode) {
	ThrowIfFailed(device.CreatePixelShader(bytecode.getBytecode(),
		bytecode.getBytecodeSize(),
		nullptr,
		shader.GetAddressOf()),
		"Failed to create pixel shader");
}

} //namespace render
