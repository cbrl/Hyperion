#pragma once

#include "EngineUtil.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl\client.h>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::ofstream;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class ShaderBase {
	public:
		ShaderBase();
		~ShaderBase();

		bool Init(ComPtr<ID3D11Device>& device, HWND hWnd, const WCHAR* vsFilename, const char* vsEntryPoint, const WCHAR* psFilename, const char* psEntryPoint);
		bool InitShader(ComPtr<ID3D11Device>& device, HWND hWnd, const WCHAR* vsFilename, const WCHAR* psFilename);
		void OutputShaderErrorMessage(ComPtr<ID3D10Blob> errorMessage, HWND hWnd, const WCHAR* shaderFilename);

		// Create the polygon layout in CreatePlygonLayout()
		virtual void CreatePolygonLayout() = 0;

		// Set descriptions for any extra buffers and initialize them in InitBuffers()
		virtual bool InitBuffers(ComPtr<ID3D11Device>& device) = 0;


		//Need to implement in derived class:
		//bool Render();
		//bool SetShaderParameters();
		//void RenderShader();
		//----------------------------------------------------
		// Render(deviceContext, indexCount, matrices, etc)
		// {
		//     SetShaderParameters(deviceContext, etc);
		//     RenderShader(deviceContext, indexCount);
		// }
		//----------------------------------------------------


	public:
		struct MatrixBuffer {
			XMMATRIX world;
			XMMATRIX view;
			XMMATRIX projection;
		};


	public:
		string m_VSEntrypoint;
		string m_PSEntrypoint;

		ComPtr<ID3D11VertexShader> m_VertexShader;
		ComPtr<ID3D11PixelShader>  m_PixelShader;
		ComPtr<ID3D11InputLayout>  m_Layout;
		ComPtr<ID3D11SamplerState> m_SamplerState;
		ComPtr<ID3D11Buffer>       m_MatrixBuffer;

		vector<D3D11_INPUT_ELEMENT_DESC> m_PolygonLayout;


	private:
		D3D11_SAMPLER_DESC               m_SamplerDesc;
		D3D11_BUFFER_DESC                m_MatrixBufferDesc;
};

