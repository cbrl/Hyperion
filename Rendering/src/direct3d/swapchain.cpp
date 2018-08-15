#include "swapchain.h"
#include "directx/directxtk.h"


SwapChain::SwapChain(HWND window,
                     DisplayConfig& config,
                     ID3D11Device& device,
                     ID3D11DeviceContext& device_context)
	: window(window)
	, display_config(config)
	, device(device)
	, device_context(device_context) {
	
	createSwapChain();
	createRenderTargetView();
}


void SwapChain::clear() const noexcept {
	Pipeline::OM::clearRTV(device_context, render_target_view.Get());
}


void SwapChain::clear(const f32 color[4]) const noexcept {
	Pipeline::OM::clearRTV(device_context, render_target_view.Get(), color);
}


void SwapChain::present() const {
	if (display_config.isVsync()) {
		swap_chain->Present(1, 0);
	}
	else {
		swap_chain->Present(0, 0);
	}
}


void SwapChain::switchMode(bool toggle_state) {
	render_target_view.Reset();

	BOOL fullscreen = FALSE;

	if (toggle_state) {
		swap_chain->GetFullscreenState(&fullscreen, nullptr);
		fullscreen = (fullscreen == TRUE) ? FALSE : TRUE;
		swap_chain->SetFullscreenState(fullscreen, nullptr);
	}

	reset();

	swap_chain->GetFullscreenState(&fullscreen, nullptr);
	display_config.setFullscreen(fullscreen == TRUE);
}


void SwapChain::createSwapChain() {

	// Swap chain desc
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.Width            = display_config.getDisplayWidth();
	desc.Height           = display_config.getDisplayHeight();
	desc.Format           = display_config.getDisplayFormat();
	desc.SampleDesc.Count = 1;
	desc.BufferUsage      = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount      = 2;
	desc.SwapEffect       = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.AlphaMode        = DXGI_ALPHA_MODE_IGNORE;
	desc.Flags            = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (!display_config.isVsync()) {
		desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}


	// Swap chain fullscreen desc
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc = {};
	fullscreen_desc.ScanlineOrdering = display_config.getDisplayScanlineOrdering();
	fullscreen_desc.Scaling          = display_config.getDisplayScaling();
	fullscreen_desc.RefreshRate      = display_config.getDisplayRefreshRate();
	fullscreen_desc.Windowed         = TRUE;


	// Get the DXGI factory
	ComPtr<IDXGIFactory2> dxgi_factory;
	ThrowIfFailed(display_config.getAdapter()->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(dxgi_factory.GetAddressOf())),
	              "Failed to get parent of dxgiFactory");

	dxgi_factory->MakeWindowAssociation(window, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

	// Create the swap chain
	ThrowIfFailed(dxgi_factory->CreateSwapChainForHwnd(&device, window, &desc, &fullscreen_desc, nullptr, swap_chain.ReleaseAndGetAddressOf()),
	              "Failed to create swapchain");

	swap_chain->SetFullscreenState(FALSE, nullptr);
}


void SwapChain::createRenderTargetView() {

	// Get the RTV back buffer texture
	ComPtr<ID3D11Texture2D> back_buffer;
	ThrowIfFailed(swap_chain->GetBuffer(0u,
	                                    __uuidof(ID3D11Texture2D),
	                                    (void**)back_buffer.GetAddressOf()),
	              "Failed to get back buffer texture");


	// Create the RTV
	ThrowIfFailed(device.CreateRenderTargetView(back_buffer.Get(),
	                                            nullptr,
	                                            render_target_view.ReleaseAndGetAddressOf()),
	              "Failed to create back buffer render target view");


	SetDebugObjectName(back_buffer.Get(), "SwapChain back buffer texture");
	SetDebugObjectName(render_target_view.Get(), "SwapChain RenderTargetView");
}


void SwapChain::reset() {

	// Swap chain flags
	UINT flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (!display_config.isVsync()) {
		flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}

	// Recreate the swap chain buffers
	swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, flags);

	// Recreate the RTV
	createRenderTargetView();
}
