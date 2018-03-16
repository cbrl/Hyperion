#pragma once

#include <fstream>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "util\engine_util.h"
#include "util\math\math.h"
#include "util\datatypes\datatypes.h"


class VertexShader {
	public:
		VertexShader() = default;
		~VertexShader() = default;

		void Init(ID3D11Device* device, const WCHAR* filename,
	              const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, size_t numElements);

		void Bind(ID3D11DeviceContext* device_context) {
			device_context->IASetInputLayout(layout.Get());
			device_context->VSSetShader(shader.Get(), nullptr, 0);
		}


	private:
		ComPtr<ID3D11VertexShader> shader;
		ComPtr<ID3D11InputLayout>  layout;
};


class PixelShader {
	public:
		PixelShader() = default;
		~PixelShader() = default;

		void Init(ID3D11Device* device, const WCHAR* filename);
		void Bind(ID3D11DeviceContext* device_context) {
			device_context->PSSetShader(shader.Get(), nullptr, 0);
		}


	private:
		ComPtr<ID3D11PixelShader> shader;
};


class ShaderError {
	public:
	static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* shaderFilename);

	private:
	ShaderError();
};