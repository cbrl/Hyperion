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

		Direct3D(HWND hWnd, int window_width, int window_height, bool fullscreen, bool vSync, bool MSAA);
		~Direct3D();

		void Init();
		void BeginScene(float red, float green, float blue, float alpha) const;
		void EndScene() const;
		void OnResize(int winWidth, int winHeight);

		ID3D11Device* GetDevice() const {
			return device.Get();
		}
		ID3D11DeviceContext* GetDeviceContext() const {
			return device_context.Get();
		}

		XMMATRIX GetWorldMatrix() const {
			return world_matrix;
		}
		XMMATRIX GetProjectionMatrix() const {
			return projection_matrix;
		}
		XMMATRIX GetOrthoMatrix() const {
			return ortho_matrix;
		}


	private:
		void ReadRefreshRate();


	private:
		ComPtr<ID3D11Device>           device;
		ComPtr<ID3D11DeviceContext>    device_context;
		ComPtr<ID3D11Debug>            debug;
		ComPtr<IDXGISwapChain>         swap_chain;
		ComPtr<ID3D11Texture2D>        depth_stencil_buffer;
		ComPtr<ID3D11DepthStencilView> depth_stencil_view;
		ComPtr<ID3D11RenderTargetView> render_target_view;
		D3D11_VIEWPORT                 window_viewport;
		D3D_DRIVER_TYPE                driver_type;

		XMMATRIX world_matrix;
		XMMATRIX projection_matrix;
		XMMATRIX ortho_matrix;

		HWND hWnd;
		int  window_width;
		int  window_height;
		bool enable_vsync;
		bool enable_fullscreen;
		bool enable_4xMSAA;
		UINT MSAA4x_quality;
		UINT numerator;
		UINT denominator;
};

