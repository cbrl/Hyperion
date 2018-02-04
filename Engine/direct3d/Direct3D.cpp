#include "stdafx.h"
#include "Direct3D.h"
#include "rendering\RenderingMgr.h"

const Direct3D* Direct3D::Get() {
	assert(RenderingMgr::Get());
	return RenderingMgr::Get()->GetD3D();
}


Direct3D::Direct3D(HWND hWnd, int windowWidth, int windowHeight, bool fullscreen, bool vSync, bool MSAA):
	hWnd(hWnd),
	windowWidth(windowWidth),
	windowHeight(windowHeight),
	enable4xMSAA(MSAA),
	enableVSync(vSync),
	enableFullscreen(fullscreen),
	driverType(D3D_DRIVER_TYPE_HARDWARE),
	MSAA4xQuality(0),
	windowViewport({})
{
}


Direct3D::~Direct3D() {
	// Restore all default settings
	if (deviceContext) {
		deviceContext->ClearState();
		deviceContext->Flush();
	}

	// Report any live objects
	#if defined(DEBUG) || defined(_DEBUG)  
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	#endif
}


bool Direct3D::Init() {
	UINT createDeviceFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		driverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&deviceContext);

	if (FAILED(hr)) {
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Create debug object
	#if defined(DEBUG) || defined(_DEBUG)  
		device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(debug.GetAddressOf()));
	#endif

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	HR(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAA4xQuality));
	assert(MSAA4xQuality > 0);

	//----------------------------------------------------------------------------------
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	//----------------------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width            = windowWidth;
	sd.BufferDesc.Height           = windowHeight;
	sd.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA
	if (enable4xMSAA) {
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = MSAA4xQuality - 1;
	}
	else {
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	// V-Sync
	if (enableVSync) {
		ReadRefreshRate();
		sd.BufferDesc.RefreshRate.Numerator   = numerator;
		sd.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		sd.BufferDesc.RefreshRate.Numerator   = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Fullscreen
	if (enableFullscreen) {
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
	HR(device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.ReleaseAndGetAddressOf()));

	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)dxgiAdapter.ReleaseAndGetAddressOf()));

	ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)dxgiFactory.ReleaseAndGetAddressOf()));

	HR(dxgiFactory->CreateSwapChain(device.Get(), &sd, swapChain.ReleaseAndGetAddressOf()));


	// Call OnResize to create the render target view, and world/view/projection matrices

	OnResize(windowWidth, windowHeight);


	return true;
}


void Direct3D::ReadRefreshRate() {
	// Create graphics interface factory
	ComPtr<IDXGIFactory>   factory;
	ComPtr<IDXGIAdapter>   adapter;
	ComPtr<IDXGIOutput>    adapterOut;
	UINT modes;

	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

	// Get list of display modes
	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &adapterOut);
	adapterOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modes, NULL);

	std::vector<DXGI_MODE_DESC> displayModeList(modes);
	adapterOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modes, displayModeList.data());

	// Find the mode that matches the screen, then store numerator and denominator for refresh rate
	for (UINT i = 0; i < modes; i++) {
		if (displayModeList[i].Width == GetSystemMetrics(SM_CXSCREEN)) {
			if (displayModeList[i].Height == GetSystemMetrics(SM_CYSCREEN)) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
}


void Direct3D::OnResize(int winWidth, int winHeight) {
	assert(deviceContext);
	assert(device);
	assert(swapChain);

	windowWidth  = winWidth;
	windowHeight = winHeight;

	renderTargetView.Reset();
	depthStencilView.Reset();
	depthStencilBuffer.Reset();

	// Resize the swap chain and recreate the render target view.

	HR(swapChain->ResizeBuffers(1, windowWidth, windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ComPtr<ID3D11Texture2D> backBuffer;
	HR(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf())));
	HR(device->CreateRenderTargetView(backBuffer.Get(), 0, renderTargetView.ReleaseAndGetAddressOf()));


	//----------------------------------------------------------------------------------
	// Create the depth/stencil buffer and view.
	//----------------------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width     = windowWidth;
	depthStencilDesc.Height    = windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (enable4xMSAA) {
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = MSAA4xQuality - 1;
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

	HR(device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer));
	HR(device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, &depthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline

	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());


	// Set the viewport transform

	windowViewport.TopLeftX = 0;
	windowViewport.TopLeftY = 0;
	windowViewport.Width    = static_cast<float>(windowWidth);
	windowViewport.Height   = static_cast<float>(windowHeight);
	windowViewport.MinDepth = 0.0f;
	windowViewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &windowViewport);


	// Set FOV and aspect ratio
	float fov = XM_PI / 4.0f;
	float aspectRatio = (float)windowWidth / (float)windowHeight;

	// Create projection matrix
	projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, zNear, zFar);

	// Create world matrix
	worldMatrix = XMMatrixIdentity();

	// Create ortho matrix for 2D rendering
	orthoMatrix = XMMatrixOrthographicLH((float)windowWidth, (float)windowHeight, zNear, zFar);
}


void Direct3D::BeginScene(float red, float green, float blue, float alpha) const {
	float color[4] = { red, green, blue, alpha };

	// Clear render taget view and depth stencil view
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void Direct3D::EndScene() const {
	if (enableVSync) {
		// If VSync is enabled, present with next frame
		HR(swapChain->Present(1, 0));
	}
	else {
		// If it's disabled, present as soon as possible
		HR(swapChain->Present(0, 0));
	}
}