#include "stdafx.h"
#include "Direct3D.h"


Direct3D::Direct3D(HWND hWnd, int windowWidth, int windowHeight, bool MSAA, bool vSync, bool fullscreen) 
:	m_hWnd(hWnd),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeight),
	m_Enable4xMSAA(MSAA),
	m_EnableVSync(vSync),
	m_EnableFullscreen(fullscreen),
	m_DriverType(D3D_DRIVER_TYPE_HARDWARE),
	m_Device(nullptr),
	m_DeviceContext(nullptr),
	m_4xMSAAQuality(0)
{
	ZeroMemory(&m_WindowViewport, sizeof(D3D11_VIEWPORT));
}


Direct3D::~Direct3D() {
	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_SwapChain);
	ReleaseCOM(m_DepthStencilBuffer);

	// Restore all default settings
	if (m_DeviceContext) {
		m_DeviceContext->ClearState();
	}

	ReleaseCOM(m_DeviceContext);
	ReleaseCOM(m_Device);
}


bool Direct3D::Init() {
	UINT createDeviceFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		m_DriverType,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&m_Device,
		&featureLevel,
		&m_DeviceContext);

	if (FAILED(hr)) {
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	HR(m_Device->CheckMultisampleQualityLevels(
	   DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality));
	assert(m_4xMSAAQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_WindowWidth;
	sd.BufferDesc.Height = m_WindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4X MSAA
	if (m_Enable4xMSAA) {
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMSAAQuality - 1;
	}
	else {
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	// V-Sync
	if (m_EnableVSync) {
		GetRefreshRate();
		sd.BufferDesc.RefreshRate.Numerator   = m_Numerator;
		sd.BufferDesc.RefreshRate.Denominator = m_Denominator;
	}
	else {
		sd.BufferDesc.RefreshRate.Numerator   = 0;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Fullscreen
	if (m_EnableFullscreen) {
		sd.Windowed = false;
	}
	else {
		sd.Windowed = true;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice *dxgiDevice = nullptr;
	HR(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter *dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory *dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_Device, &sd, &m_SwapChain));

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	OnResize();

	return true;
}


void Direct3D::GetRefreshRate() {
	// Create graphics interface factory
	IDXGIFactory   *factory;
	IDXGIAdapter   *adapter;
	IDXGIOutput    *adapterOut;
	DXGI_MODE_DESC *displayModeList;
	UINT            modes;

	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

	// Get list of display modes, find the one that matches the screen, then store numerator and denominator for refresh rate
	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &adapterOut);
	adapterOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modes, NULL);
	displayModeList = new DXGI_MODE_DESC[modes];
	adapterOut->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modes, displayModeList);
	for (UINT i = 0; i < modes; i++) {
		if (displayModeList[i].Width == GetSystemMetrics(SM_CXSCREEN)) {
			if (displayModeList[i].Height == GetSystemMetrics(SM_CYSCREEN)) {
				m_Numerator = displayModeList[i].RefreshRate.Numerator;
				m_Denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	ReleaseCOM(factory);
	ReleaseCOM(adapter);
	ReleaseCOM(adapterOut);
}


void Direct3D::OnResize() {
	assert(m_DeviceContext);
	assert(m_Device);
	assert(m_SwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_DepthStencilBuffer);


	// Resize the swap chain and recreate the render target view.

	HR(m_SwapChain->ResizeBuffers(1, m_WindowWidth, m_WindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_Device->CreateRenderTargetView(backBuffer, 0, &m_RenderTargetView));
	ReleaseCOM(backBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_WindowWidth;
	depthStencilDesc.Height = m_WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (m_Enable4xMSAA) {
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMSAAQuality - 1;
	}
	// No MSAA
	else {
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_Device->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer));
	HR(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


	// Set the viewport transform

	m_WindowViewport.TopLeftX = 0;
	m_WindowViewport.TopLeftY = 0;
	m_WindowViewport.Width = static_cast<float>(m_WindowWidth);
	m_WindowViewport.Height = static_cast<float>(m_WindowHeight);
	m_WindowViewport.MinDepth = 0.0f;
	m_WindowViewport.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &m_WindowViewport);
}
