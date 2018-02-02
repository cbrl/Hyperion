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
		static const Direct3D* Get();

		Direct3D(HWND hWnd, int windowWidth, int windowHeight, bool fullscreen, bool vSync, bool MSAA);
		~Direct3D();

		bool Init();
		void BeginScene(float red, float green, float blue, float alpha) const;
		void EndScene() const;
		void OnResize(int winWidth, int winHeight);

		ComPtr<ID3D11Device> GetDevice() const {
			return device;
		}
		ComPtr<ID3D11DeviceContext> GetDeviceContext() const {
			return deviceContext;
		}

		XMMATRIX GetWorldMatrix() const {
			return worldMatrix;
		}
		XMMATRIX GetProjectionMatrix() const {
			return projectionMatrix;
		}
		XMMATRIX GetOrthoMatrix() const {
			return orthoMatrix;
		}


	private:
		void ReadRefreshRate();


	private:
		ComPtr<ID3D11Device>           device;
		ComPtr<ID3D11DeviceContext>    deviceContext;
		ComPtr<ID3D11Debug>            debug;
		ComPtr<IDXGISwapChain>         swapChain;
		ComPtr<ID3D11Texture2D>        depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		D3D11_VIEWPORT                 windowViewport;
		D3D_DRIVER_TYPE                driverType;

		XMMATRIX worldMatrix;
		XMMATRIX projectionMatrix;
		XMMATRIX orthoMatrix;

		HWND hWnd;
		int  windowWidth;
		int  windowHeight;
		bool enableVSync;
		bool enableFullscreen;
		bool enable4xMSAA;
		UINT MSAA4xQuality;
		UINT numerator;
		UINT denominator;
};

