#include "stdafx.h"
#include "Direct3D.h"


Direct3D::Direct3D(HWND hWnd, int windowWidth, int windowHeight, bool MSAA, bool vSync, bool fullscreen):
	m_hWnd(hWnd),
	m_WindowWidth(windowWidth),
	m_WindowHeight(windowHeight),
	m_Enable4xMSAA(MSAA),
	m_EnableVSync(vSync),
	m_EnableFullscreen(fullscreen),
	m_DriverType(D3D_DRIVER_TYPE_HARDWARE),
	m_4xMSAAQuality(0)
{
	ZeroMemory(&m_WindowViewport, sizeof(D3D11_VIEWPORT));
}


Direct3D::~Direct3D() {
	// Restore all default settings
	if (m_DeviceContext) {
		m_DeviceContext->ClearState();
	}
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

	// Check 4X MSAA quality support for our back m_Buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	HR(m_Device->CheckMultisampleQualityLevels(
	   DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMSAAQuality));
	assert(m_4xMSAAQuality > 0);

	//----------------------------------------------------------------------------------
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	//----------------------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width            = m_WindowWidth;
	sd.BufferDesc.Height           = m_WindowHeight;
	sd.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

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

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = m_hWnd;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	HR(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.ReleaseAndGetAddressOf()));

	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)dxgiAdapter.ReleaseAndGetAddressOf()));

	ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)dxgiFactory.ReleaseAndGetAddressOf()));

	HR(dxgiFactory->CreateSwapChain(m_Device.Get(), &sd, m_SwapChain.ReleaseAndGetAddressOf()));

	OnResize(m_WindowWidth, m_WindowHeight);


	return true;
}


void Direct3D::GetRefreshRate() {
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
				m_Numerator = displayModeList[i].RefreshRate.Numerator;
				m_Denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
}


ComPtr<ID3D11Device> Direct3D::GetDevice() {
	return m_Device;
}


ComPtr<ID3D11DeviceContext> Direct3D::GetDeviceContext() {
	return m_DeviceContext;
}


XMMATRIX Direct3D::GetWorldMatrix() {
	return m_WorldMatrix;
}


XMMATRIX Direct3D::GetProjectionMatrix() {
	return m_ProjectionMatrix;
}


XMMATRIX Direct3D::GetOrthoMatrix() {
	return m_OrthoMatrix;
}


void Direct3D::OnResize(int windowWidth, int windowHeight) {
	assert(m_DeviceContext);
	assert(m_Device);
	assert(m_SwapChain);

	m_WindowWidth  = windowWidth;
	m_WindowHeight = windowHeight;

	m_RenderTargetView.Reset();
	m_DepthStencilView.Reset();
	m_DepthStencilBuffer.Reset();

	// Resize the swap chain and recreate the render target view.

	HR(m_SwapChain->ResizeBuffers(1, m_WindowWidth, m_WindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ComPtr<ID3D11Texture2D> backBuffer;
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.ReleaseAndGetAddressOf())));
	HR(m_Device->CreateRenderTargetView(backBuffer.Get(), 0, m_RenderTargetView.ReleaseAndGetAddressOf()));


	//----------------------------------------------------------------------------------
	// Create the depth/stencil m_Buffer and view.
	//----------------------------------------------------------------------------------
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
	HR(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), 0, &m_DepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());


	// Set the viewport transform

	m_WindowViewport.TopLeftX = 0;
	m_WindowViewport.TopLeftY = 0;
	m_WindowViewport.Width = static_cast<float>(m_WindowWidth);
	m_WindowViewport.Height = static_cast<float>(m_WindowHeight);
	m_WindowViewport.MinDepth = 0.0f;
	m_WindowViewport.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &m_WindowViewport);


	// Set FOV and aspect ratio
	float fov = XM_PI / 4.0f;
	float aspectRatio = (float)m_WindowWidth / (float)m_WindowHeight;

	// Create projection matrix
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, zNear, zFar);

	// Create world matrix
	m_WorldMatrix = XMMatrixIdentity();

	// Create ortho matrix for 2D rendering
	m_OrthoMatrix = XMMatrixOrthographicLH((float)m_WindowWidth, (float)m_WindowHeight, zNear, zFar);
}


void Direct3D::BeginScene(float red, float green, float blue, float alpha) {
	float color[4] = { red, green, blue, alpha };

	// Clear render taget view and depth stencil view
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void Direct3D::EndScene() {
	if (m_EnableVSync) {
		// If VSync is enabled, present with next frame
		HR(m_SwapChain->Present(1, 0));
	}
	else {
		// If it's disabled, present as soon as possible
		HR(m_SwapChain->Present(0, 0));
	}
}