#include "stdafx.h"
#include "direct3d.h"
#include "rendering\rendering_mgr.h"

const Direct3D* Direct3D::Get() {
	assert(RenderingMgr::Get());
	return RenderingMgr::Get()->GetD3D();
}


Direct3D::Direct3D(HWND hWnd, i32 window_width, i32 window_height, bool fullscreen, bool vSync, bool MSAA)
	: hWnd(hWnd)
	, window_width(window_width)
	, window_height(window_height)
	, enable_4xMSAA(MSAA)
	, enable_vsync(vSync)
	, enable_fullscreen(fullscreen)
	, driver_type(D3D_DRIVER_TYPE_HARDWARE)
	, MSAA4x_quality(0)
	, window_viewport({})
{
	Init();
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


void Direct3D::Init() {
	u32 createDeviceFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		driver_type,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&device_context);

	DX::ThrowIfFailed(hr, "D3D11CreateDevice Failed");

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		DX::ThrowIfFailed(E_FAIL, "Direct3D Feature Level 11 unsupported");
	}

	// Create debug object
	#if defined(DEBUG) || defined(_DEBUG)  
		device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(debug.GetAddressOf()));
	#endif

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	DX::ThrowIfFailed(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAA4x_quality),
	                  "CheckMultisampleQualityLevels() failed");
	assert(MSAA4x_quality > 0);

	//----------------------------------------------------------------------------------
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	//----------------------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width            = window_width;
	sd.BufferDesc.Height           = window_height;
	sd.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA
	if (enable_4xMSAA) {
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = MSAA4x_quality - 1;
	}
	else {
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	// V-Sync
	if (enable_vsync) {
		ReadRefreshRate();
		sd.BufferDesc.RefreshRate.Numerator   = refresh_rate_numerator;
		sd.BufferDesc.RefreshRate.Denominator = refresh_rate_denominator;
	}
	else {
		sd.BufferDesc.RefreshRate.Numerator   = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Fullscreen
	if (enable_fullscreen) {
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
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	DX::ThrowIfFailed(device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.ReleaseAndGetAddressOf()),
	                  "QueryInterface failed on dxgiDevice");

	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	DX::ThrowIfFailed(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)dxgiAdapter.ReleaseAndGetAddressOf()),
	                  "Failed to get parent of dxgiDevice");

	ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	DX::ThrowIfFailed(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)dxgiFactory.ReleaseAndGetAddressOf()),
	                  "Failed to get parent of dxgiFactory");

	DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(device.Get(), &sd, swap_chain.ReleaseAndGetAddressOf()),
	                  "Failed to create swapchain");


	// Call OnResize to create the render target view, and world/view/projection matrices
	OnResize(window_width, window_height);
}


void Direct3D::ReadRefreshRate() {
	// Create graphics interface factory
	ComPtr<IDXGIFactory>   factory;
	ComPtr<IDXGIAdapter>   adapter;
	ComPtr<IDXGIOutput>    adapterOut;
	u32 modes;

	DX::ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory),
	                  "Failed to create dxgiFactory");


	// Get list of display modes

	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &adapterOut);
	adapterOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modes, NULL);

	std::vector<DXGI_MODE_DESC> displayModeList(modes);
	adapterOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modes, displayModeList.data());


	// Find the mode that matches the screen, then store numerator and denominator for refresh rate

	for (u32 i = 0; i < modes; i++) {
		if (displayModeList[i].Width == GetSystemMetrics(SM_CXSCREEN)) {
			if (displayModeList[i].Height == GetSystemMetrics(SM_CYSCREEN)) {
				refresh_rate_numerator = displayModeList[i].RefreshRate.Numerator;
				refresh_rate_denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
}


void Direct3D::OnResize(i32 winWidth, i32 winHeight) {
	assert(device_context);
	assert(device);
	assert(swap_chain);

	window_width  = winWidth;
	window_height = winHeight;

	render_target_view.Reset();
	depth_stencil_view.Reset();
	depth_stencil_buffer.Reset();

	// Resize the swap chain and recreate the render target view

	DX::ThrowIfFailed(swap_chain->ResizeBuffers(1, window_width, window_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0),
	                  "Failed to resize swapchain buffers");

	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf())),
	                  "Failed to get backbuffer");
	DX::ThrowIfFailed(device->CreateRenderTargetView(backBuffer.Get(), 0, render_target_view.ReleaseAndGetAddressOf()),
	                  "Failed to create render target view");


	// Set the name the backbuffer and render target view for debugging purposes
	SetDebugObjectName(backBuffer.Get(), "D3D BackBuffer");
	SetDebugObjectName(render_target_view.Get(), "D3D RenderTargetView");


	//----------------------------------------------------------------------------------
	// Create the depth/stencil buffer and view.
	//----------------------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width     = window_width;
	depthStencilDesc.Height    = window_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (enable_4xMSAA) {
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = MSAA4x_quality - 1;
	}
	// No MSAA
	else {
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags      = 0;

	DX::ThrowIfFailed(device->CreateTexture2D(&depthStencilDesc, 0, &depth_stencil_buffer),
	                  "Failed to create depth stencil buffer");
	DX::ThrowIfFailed(device->CreateDepthStencilView(depth_stencil_buffer.Get(), 0, &depth_stencil_view),
	                  "Failed to create depth stencil view");

	// Set object names for debugging
	SetDebugObjectName(depth_stencil_buffer.Get(), "D3D DepthStencilBuffer");
	SetDebugObjectName(depth_stencil_view.Get(), "D3DDepthStencilView");


	// Bind the render target view and depth/stencil view to the pipeline

	device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());


	// Set the viewport transform

	window_viewport.TopLeftX = 0;
	window_viewport.TopLeftY = 0;
	window_viewport.Width    = static_cast<float>(window_width);
	window_viewport.Height   = static_cast<float>(window_height);
	window_viewport.MinDepth = 0.0f;
	window_viewport.MaxDepth = 1.0f;

	device_context->RSSetViewports(1, &window_viewport);
}


void Direct3D::BeginScene(float red, float green, float blue, float alpha) const {
	float color[4] = { red, green, blue, alpha };

	// Clear render taget view and depth stencil view
	device_context->ClearRenderTargetView(render_target_view.Get(), color);
	device_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void Direct3D::EndScene() const {
	if (enable_vsync) {
		// If VSync is enabled, present with next frame
		DX::ThrowIfFailed(swap_chain->Present(1, 0),
		                  "Failed to present frame");
	}
	else {
		// If it's disabled, present as soon as possible
		DX::ThrowIfFailed(swap_chain->Present(0, 0),
		                  "Failed to present frame");
	}
}