#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <dxgi.h>
#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"

using namespace DirectX;

class Direct3D {
	public:
		static const Direct3D* Get();

		Direct3D(HWND hWnd, i32 window_width, i32 window_height, bool fullscreen, bool vSync, bool MSAA);
		~Direct3D();

		void BeginScene(float red, float green, float blue, float alpha) const;
		void EndScene() const;
		void OnResize(i32 winWidth, i32 winHeight);

		ID3D11Device* GetDevice() const {
			return device.Get();
		}
		ID3D11DeviceContext* GetDeviceContext() const {
			return device_context.Get();
		}


	private:
		void Init();
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

		HWND hWnd;
		i32  window_width;
		i32  window_height;
		bool enable_vsync;
		bool enable_fullscreen;
		bool enable_4xMSAA;
		u32  MSAA4x_quality;
		u32  refresh_rate_numerator;
		u32  refresh_rate_denominator;
};

