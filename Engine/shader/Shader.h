#pragma once

#include <fstream>
#include <wrl\client.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <VertexTypes.h>
#include "util\EngineUtil.h"

using std::string;
using std::vector;
using std::ofstream;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class Shader {
	public:
		Shader();
		~Shader();

		void Init(HWND hWnd, ID3D11Device* device, const WCHAR* vsFilename, const WCHAR* psFilename,
		          const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, size_t numElements);

		void BindShader(ID3D11DeviceContext* device_context);


	private:
		void OutputShaderErrorMessage(HWND hWnd, ID3D10Blob* errorMessage, const WCHAR* shaderFilename);


	private:
		ComPtr<ID3D11VertexShader> vertex_shader;
		ComPtr<ID3D11InputLayout>  vertex_layout;
		ComPtr<ID3D11PixelShader>  pixel_shader;
};

