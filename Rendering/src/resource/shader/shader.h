#pragma once

#include "directx/d3d11.h"
#include "resource/resource.h"
#include "resource/shader/shader_bytecode.h"


class VertexShader final : public Resource<VertexShader> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	VertexShader(const std::wstring& guid,
	             ID3D11Device& device,
	             const ShaderBytecode& bytecode,
	             const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
	             size_t numElements);

	VertexShader(const VertexShader& shader) = delete;
	VertexShader(VertexShader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~VertexShader() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	VertexShader& operator=(const VertexShader& shader) = delete;
	VertexShader& operator=(VertexShader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Bind the vertex shader to a pipeline stage
	void bind(ID3D11DeviceContext& device_context) const {
		if (layout) Pipeline::IA::bindInputLayout(device_context, layout.Get());
		Pipeline::VS::bindShader(device_context, shader.Get(), nullptr, 0);
	}


private:
	void createShader(ID3D11Device& device,
					  const ShaderBytecode& bytecode,
					  const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
					  size_t numElements);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ID3D11VertexShader> shader;
	ComPtr<ID3D11InputLayout> layout;
};




template<typename ShaderT, typename StageT>
class Shader final : public Resource<Shader<ShaderT, StageT>> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Shader(const std::wstring& guid,
		   ID3D11Device& device,
		   const ShaderBytecode& bytecode);

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
		StageT::bindShader(device_context, shader.Get(), nullptr, 0);
	}


private:
	void createShader(ID3D11Device& device, const ShaderBytecode& bytecode);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ShaderT> shader;
};

using ComputeShader  = Shader<ID3D11ComputeShader, Pipeline::CS>;
using DomainShader   = Shader<ID3D11DomainShader, Pipeline::DS>;
using GeometryShader = Shader<ID3D11GeometryShader, Pipeline::GS>;
using HullShader     = Shader<ID3D11HullShader, Pipeline::HS>;
using PixelShader    = Shader<ID3D11PixelShader, Pipeline::PS>;


#include "shader.tpp"
