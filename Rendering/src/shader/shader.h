#pragma once

#include "directx/d3d11.h"
#include <d3dcompiler.h>

#include "datatypes/datatypes.h"
#include "pipeline.h"


class VertexShader final {
public:
	explicit VertexShader(ID3D11Device& device,
	                      const wchar_t* filename,
	                      const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
	                      size_t numElements);

	explicit VertexShader(ID3D11Device& device,
	                      const BYTE* buffer,
	                      size_t size,
	                      const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
	                      size_t numElements);

	~VertexShader() = default;


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
	ComPtr<ID3D11VertexShader> shader;
	ComPtr<ID3D11InputLayout> layout;
};


class PixelShader final {
public:
	explicit PixelShader(ID3D11Device& device,
	                     const wchar_t* filename);

	explicit PixelShader(ID3D11Device& device,
	                     const BYTE* buffer,
	                     size_t size);

	~PixelShader() = default;


	// Bind the vertex shader to a pipeline stage
	void bind(ID3D11DeviceContext& device_context) const {
		Pipeline::PS::bindShader(device_context, shader.Get(), nullptr, 0);
	}


private:
	void createShader(ID3D11Device& device,
                      const void* buffer,
                      size_t size);


private:
	ComPtr<ID3D11PixelShader> shader;
};


static void OutputShaderErrorMessage(ID3D10Blob* error_message, const WCHAR* shader_filename);
