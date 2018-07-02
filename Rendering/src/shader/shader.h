#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "pipeline.h"


class VertexShader final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	VertexShader(ID3D11Device& device,
	             const wchar_t* filename,
	             const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
	             size_t numElements);

	VertexShader(ID3D11Device& device,
	             const BYTE* buffer,
	             size_t size,
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
	                  const void* buffer,
	                  size_t size,
	                  const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
	                  size_t numElements);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ID3D11VertexShader> shader;
	ComPtr<ID3D11InputLayout> layout;
};


class PixelShader final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	PixelShader(ID3D11Device& device,
	            const wchar_t* filename);

	PixelShader(ID3D11Device& device,
	            const BYTE* buffer,
	            size_t size);

	PixelShader(const PixelShader& shader) = delete;
	PixelShader(PixelShader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~PixelShader() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	PixelShader& operator=(const PixelShader& shader) = delete;
	PixelShader& operator=(PixelShader&& shader) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Bind the vertex shader to a pipeline stage
	void bind(ID3D11DeviceContext& device_context) const {
		Pipeline::PS::bindShader(device_context, shader.Get(), nullptr, 0);
	}


private:
	void createShader(ID3D11Device& device,
                      const void* buffer,
                      size_t size);


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ID3D11PixelShader> shader;
};


static void OutputShaderErrorMessage(ID3D10Blob* error_message, const WCHAR* shader_filename);
