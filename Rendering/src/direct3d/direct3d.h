#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <dxgi.h>
#include "datatypes/datatypes.h"
#include "display_config.h"

using namespace DirectX;


class Direct3D final {
public:
	Direct3D(HWND window, DisplayConfig config);

	~Direct3D();

	// Clear the backbuffer
	void clear() const;

	// Clear the backbuffer with a specified color
	void clear(const float color[4]) const;

	// Present the current frame
	void presentFrame() const;

	// Resize the depth/stencil/back buffers
	void resizeBuffers(u32 win_width, u32 win_height);

	void bindDefaultRenderTarget() const {
		device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
	}

	ID3D11Device& getDevice() const {
		return *device.Get();
	}

	ID3D11DeviceContext& getDeviceContext() const {
		return *device_context.Get();
	}


private:
	void init();
	void readRefreshRate(UINT& numerator, UINT& denominator) const;


private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> device_context;
	ComPtr<ID3D11Debug> debug;
	ComPtr<IDXGISwapChain> swap_chain;
	ComPtr<ID3D11Texture2D> depth_stencil_buffer;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	ComPtr<ID3D11RenderTargetView> render_target_view;
	D3D_DRIVER_TYPE driver_type;

	HWND hWnd;
	u32  window_width;
	u32  window_height;
	bool enable_vsync;
	bool enable_fullscreen;
	bool enable_4x_msaa;
	u32  msaa_4x_quality;
};
