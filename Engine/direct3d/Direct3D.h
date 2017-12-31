#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <wrl\client.h>
#include <d3d11.h>
#include <dxgi.h>
#include "util\EngineUtil.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

static const float zNear = 0.1f;
static const float zFar  = 1000.0f;

class Direct3D {
	public:
		Direct3D(HWND hWnd, int windowWidth, int windowHeight, bool MSAA, bool vSync, bool fullscreen);
		~Direct3D();

		bool Init();
		void BeginScene(float red, float green, float blue, float alpha);
		void EndScene();
		void OnResize(int windowWidth, int windowHeight);

		ComPtr<ID3D11Device>        GetDevice();
		ComPtr<ID3D11DeviceContext> GetDeviceContext();

		XMMATRIX GetWorldMatrix();
		XMMATRIX GetProjectionMatrix();
		XMMATRIX GetOrthoMatrix();


	private:
		void GetRefreshRate();


	private:
		ComPtr<ID3D11Device>           m_Device;
		ComPtr<ID3D11DeviceContext>    m_DeviceContext;
		ComPtr<IDXGISwapChain>         m_SwapChain;
		ComPtr<ID3D11Texture2D>        m_DepthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		D3D11_VIEWPORT                 m_WindowViewport;
		D3D_DRIVER_TYPE                m_DriverType;

		XMMATRIX m_WorldMatrix;
		XMMATRIX m_ProjectionMatrix;
		XMMATRIX m_OrthoMatrix;

		HWND m_hWnd;
		int  m_WindowWidth;
		int  m_WindowHeight;
		bool m_EnableVSync;
		bool m_EnableFullscreen;
		bool m_Enable4xMSAA;
		UINT m_4xMSAAQuality;
		UINT m_Numerator;
		UINT m_Denominator;
};

