#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"


class VertexShader {
	public:
		VertexShader(ID3D11Device* device,
					 const WCHAR* filename,
					 const D3D11_INPUT_ELEMENT_DESC* inputElementDesc,
					 size_t numElements);

		~VertexShader() = default;

		void Bind(ID3D11DeviceContext* device_context) {
			Pipeline::IA::BindInputLayout(device_context, layout.Get());
			Pipeline::VS::BindShader(device_context, shader.Get(), nullptr, 0);
		}


	private:
		ComPtr<ID3D11VertexShader> shader;
		ComPtr<ID3D11InputLayout>  layout;
};


class PixelShader {
	public:
		PixelShader(ID3D11Device* device,
					const WCHAR* filename);

		~PixelShader() = default;

		void Bind(ID3D11DeviceContext* device_context) {
			Pipeline::PS::BindShader(device_context, shader.Get(), nullptr, 0);
		}


	private:
		ComPtr<ID3D11PixelShader> shader;
};


static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* shaderFilename);