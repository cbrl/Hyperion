#include "direct3d.h"

#include "directx/directxtk.h"
#include "pipeline.h"


Direct3D::Direct3D(HWND window, DisplayConfig config)
	: hWnd(window)
	, display_config(std::move(config))
	, enable_4x_msaa(display_config.getAAType() == AAType::msaa_4x)
	, msaa_4x_quality(0) {

	init();
}


Direct3D::~Direct3D() {
	// Restore all default settings
	if (device_context) {
		device_context->ClearState();
		device_context->Flush();
	}

	// Report any live objects
	#if defined(DEBUG) || defined(_DEBUG)
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	#endif
}


void Direct3D::init() {

	u32 create_device_flags = 0;
	#if defined(DEBUG) || defined(_DEBUG)
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL feature_level;
	const HRESULT hr = D3D11CreateDevice(display_config.getAdapter(),
	                                     D3D_DRIVER_TYPE_UNKNOWN,
	                                     nullptr, // no software device
	                                     create_device_flags,
	                                     nullptr,
	                                     0, // default feature level array
	                                     D3D11_SDK_VERSION,
	                                     device.GetAddressOf(),
	                                     &feature_level,
	                                     device_context.GetAddressOf());

	ThrowIfFailed(hr, "D3D11CreateDevice Failed");

	ThrowIfFailed(feature_level == D3D_FEATURE_LEVEL_11_0,
	              "Direct3D Feature Level 11 unsupported");

	// Create debug object
	#if defined(DEBUG) || defined(_DEBUG)
	device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(debug.GetAddressOf()));
	#endif

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	ThrowIfFailed(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_4x_quality),
	              "CheckMultisampleQualityLevels() failed");

	assert(msaa_4x_quality > 0);

	//----------------------------------------------------------------------------------
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	//----------------------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width            = display_config.getDisplayDesc().Width;
	sd.BufferDesc.Height           = display_config.getDisplayDesc().Height;
	sd.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA
	if (enable_4x_msaa) {
		sd.SampleDesc.Count   = 4;
		sd.SampleDesc.Quality = msaa_4x_quality - 1;
	}
	else {
		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;
	}

	// V-Sync
	if (!display_config.isVsync()) {
		// Read refresh rate into buffer description
		sd.BufferDesc.RefreshRate.Numerator   = display_config.getDisplayDesc().RefreshRate.Numerator;
		sd.BufferDesc.RefreshRate.Denominator = display_config.getDisplayDesc().RefreshRate.Denominator;
	}
	else {
		sd.BufferDesc.RefreshRate.Numerator   = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Fullscreen
	if (display_config.isFullscreen()) {
		sd.Windowed = false;
	}
	else {
		sd.Windowed = true;
	}

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = hWnd;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device. If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	ComPtr<IDXGIFactory> dxgi_factory;
	ThrowIfFailed(display_config.getAdapter()->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgi_factory.GetAddressOf())),
	              "Failed to get parent of dxgiFactory");

	ThrowIfFailed(dxgi_factory->CreateSwapChain(device.Get(), &sd, swap_chain.GetAddressOf()),
	              "Failed to create swapchain");


	// Call resize function to create the render target view, and world/view/projection matrices
	resizeBuffers(display_config.getDisplayDesc().Width, display_config.getDisplayDesc().Height);
}


void Direct3D::resizeBuffers(u32 win_width, u32 win_height) {

	assert(device_context);
	assert(device);
	assert(swap_chain);

	render_target_view.Reset();
	depth_stencil_view.Reset();
	depth_stencil_buffer.Reset();

	// Resize the swap chain and recreate the render target view

	ThrowIfFailed(swap_chain->ResizeBuffers(1, win_width, win_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0),
	              "Failed to resize swapchain buffers");

	ComPtr<ID3D11Texture2D> back_buffer;
	ThrowIfFailed(swap_chain->GetBuffer(0,
	                                    __uuidof(ID3D11Texture2D),
	                                    reinterpret_cast<void**>(back_buffer.ReleaseAndGetAddressOf())),
	              "Failed to get backbuffer");
	ThrowIfFailed(device->CreateRenderTargetView(back_buffer.Get(),
	                                             nullptr,
	                                             render_target_view.ReleaseAndGetAddressOf()),
	              "Failed to create render target view");


	// Set the name the backbuffer and render target view for debugging purposes
	SetDebugObjectName(back_buffer.Get(), "D3D BackBuffer");
	SetDebugObjectName(render_target_view.Get(), "D3D RenderTargetView");


	//----------------------------------------------------------------------------------
	// Create the depth/stencil buffer and view.
	//----------------------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC depth_stencil_desc;

	depth_stencil_desc.Width     = win_width;
	depth_stencil_desc.Height    = win_height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (enable_4x_msaa) {
		depth_stencil_desc.SampleDesc.Count   = 4;
		depth_stencil_desc.SampleDesc.Quality = msaa_4x_quality - 1;
	}
		// No MSAA
	else {
		depth_stencil_desc.SampleDesc.Count   = 1;
		depth_stencil_desc.SampleDesc.Quality = 0;
	}

	depth_stencil_desc.Usage          = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags      = 0;

	ThrowIfFailed(device->CreateTexture2D(&depth_stencil_desc, nullptr, &depth_stencil_buffer),
	              "Failed to create depth stencil buffer");
	ThrowIfFailed(device->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, &depth_stencil_view),
	              "Failed to create depth stencil view");

	// Set object names for debugging
	SetDebugObjectName(depth_stencil_buffer.Get(), "D3D DepthStencilBuffer");
	SetDebugObjectName(depth_stencil_view.Get(), "D3DDepthStencilView");


	// Bind the render target view and depth/stencil view to the pipeline

	Pipeline::OM::bindRTVs(*device_context.Get(), 1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
}


void Direct3D::clear() const {
	Pipeline::OM::clearRTV(*device_context.Get(), render_target_view.Get());
	Pipeline::OM::clearDSV(*device_context.Get(), depth_stencil_view.Get());
}


void Direct3D::clear(const f32 color[4]) const {
	Pipeline::OM::clearRTV(*device_context.Get(), render_target_view.Get(), color);
	Pipeline::OM::clearDSV(*device_context.Get(), depth_stencil_view.Get());
}


void Direct3D::presentFrame() const {
	if (display_config.isVsync()) {
		// If VSync is enabled, present with next frame
		ThrowIfFailed(swap_chain->Present(1, 0),
		              "Failed to present frame");
	}
	else {
		// If it's disabled, present as soon as possible
		ThrowIfFailed(swap_chain->Present(0, 0),
		              "Failed to present frame");
	}
}
