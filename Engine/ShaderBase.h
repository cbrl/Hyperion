#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl\client.h>
#include <fstream>
#include <string>
#include <vector>
#include "EngineUtil.h"

using std::string;
using std::vector;
using std::ofstream;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class ShaderBase {
	public:
		ShaderBase();
		~ShaderBase();

		bool Init(const ComPtr<ID3D11Device>& device, HWND hWnd, const WCHAR* vsFilename, const WCHAR* psFilename);
		bool InitShader(const ComPtr<ID3D11Device>& device, HWND hWnd, const WCHAR* vsFilename, const WCHAR* psFilename);
		void OutputShaderErrorMessage(ComPtr<ID3D10Blob> errorMessage, HWND hWnd, const WCHAR* shaderFilename);

		// Create the polygon layout in CreatePlygonLayout()
		virtual void CreatePolygonLayout() = 0;

		// Set descriptions for any extra buffers and initialize them in InitBuffers()
		virtual bool InitBuffers(const ComPtr<ID3D11Device>& device) = 0;

		// Set shader resources and buffers
		virtual void SetShader(const ComPtr<ID3D11DeviceContext>& deviceContext) = 0;
		virtual void SetCBuffers(const ComPtr<ID3D11DeviceContext>& deviceContext) = 0;

		// Render shader
		virtual void Render(const ComPtr<ID3D11DeviceContext>& deviceContext, int indexCount) = 0;


		// Need to implement in derived class:
		//   void SetParameters(...);
		//-----------------------Usage------------------------
		// SetShader();
		// SetCBuffers();
		// SetParameters(deviceContext, matrices, lights, etc...);
		// Render();
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

