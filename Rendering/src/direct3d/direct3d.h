#pragma once

#include "directx/d3d11.h"
#include "datatypes/datatypes.h"
#include "display/display_config.h"


class Direct3D final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	Direct3D(HWND window, DisplayConfig config);
	Direct3D(const Direct3D& d3d) = delete;
	Direct3D(Direct3D&& d3d) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~Direct3D();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	Direct3D& operator=(const Direct3D& d3d) = delete;
	Direct3D& operator=(Direct3D&& d3d) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Clear the backbuffer
	void clear() const;

	// Clear the backbuffer with a specified color
	void clear(const f32 color[4]) const;

	// Present the current frame
	void presentFrame() const;

	// Resize the depth/stencil/back buffers
	void resizeBuffers(u32 win_width, u32 win_height);

	void bindDefaultRenderTarget() const {
		device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
	}

	[[nodiscard]]
	ID3D11Device& getDevice() const {
		return *device.Get();
	}

	[[nodiscard]]
	ID3D11DeviceContext& getDeviceContext() const {
		return *device_context.Get();
	}


private:
	void init();


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> device_context;
	ComPtr<ID3D11Debug> debug;
	ComPtr<IDXGISwapChain> swap_chain;
	ComPtr<ID3D11Texture2D> depth_stencil_buffer;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	ComPtr<ID3D11RenderTargetView> render_target_view;

	HWND hWnd;
	DisplayConfig display_config;
	bool enable_4x_msaa;
	u32  msaa_4x_quality;
};
