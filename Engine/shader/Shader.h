#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <VertexTypes.h>
#include <Windows.h>
#include <wrl\client.h>
#include <fstream>
#include <string>
#include <vector>
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

		bool Init(HWND hWnd, const ComPtr<ID3D11Device>& device, const WCHAR* vsFilename, const WCHAR* psFilename,
		          const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, size_t numElements);

		void BindShader(const ComPtr<ID3D11DeviceContext>& deviceContext);


	private:
		void OutputShaderErrorMessage(HWND hWnd, ID3D10Blob* errorMessage, const WCHAR* shaderFilename);


	private:
		ComPtr<ID3D11VertexShader> m_VertexShader;
		ComPtr<ID3D11InputLayout>  m_VertexLayout;
		ComPtr<ID3D11PixelShader>  m_PixelShader;
};

