module;

#include "directx/d3d11.h"
#include "directx/directxtk.h"

export module rendering.swapchain;

import rendering.direct3d;
import rendering.display_config;
import rendering.pipeline;


export namespace render {

class SwapChain final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	SwapChain(gsl::not_null<HWND> window,
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

	SwapChain(const SwapChain& swap_chain) = delete;
	SwapChain(SwapChain&& swap_chain) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~SwapChain() {
		// Ensure the swap chain isn't in fullscreen mode on destruction,
		// as DirectX will not reset the state itself.
		if (swap_chain)
			swap_chain->SetFullscreenState(FALSE, nullptr);
	}


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	SwapChain& operator=(const SwapChain& swap_chain) = delete;
	SwapChain& operator=(SwapChain&& swap_chain) = delete;


	//----------------------------------------------------------------------------------
	// Member Functions - Render Target View
	//----------------------------------------------------------------------------------

	// Clear the render target view
	void clear() const noexcept {
		Pipeline::OM::clearRTV(device_context, render_target_view.Get());
	}

	void clear(const f32 color[4]) const noexcept {
		Pipeline::OM::clearRTV(device_context, render_target_view.Get(), color);
	}

	// Get a pointer to the render target view
	ID3D11RenderTargetView* getRTV() const noexcept {
		return render_target_view.Get();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Frame
	//----------------------------------------------------------------------------------

	// Present the frame
	void present() const {
		if (display_config.isVsync()) {
			swap_chain->Present(1, 0);
		}
		else {
			swap_chain->Present(0, 0);
		}
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Fullscreen
	//----------------------------------------------------------------------------------

	// Switch the fullscreen state. toggle_state determines if the
	// swap chain's fullscreen state also needs to be changed.
	void switchMode(bool toggle_state) {
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

	[[nodiscard]]
	bool isFullscreen() const noexcept {
		BOOL fullscreen = FALSE;
		swap_chain->GetFullscreenState(&fullscreen, nullptr);
		return fullscreen == TRUE;
	}

	[[nodiscard]]
	bool lostMode() const noexcept {
		return display_config.isFullscreen() != this->isFullscreen();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Reset
	//----------------------------------------------------------------------------------

	// Recreate the swap chain and render target view
	void reset() {
		// Swap chain flags
		UINT flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		if (not display_config.isVsync()) {
			flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		}

		// Release the RTV
		render_target_view.Reset();

		// Recreate the swap chain buffers
		BOOL fullscreen = FALSE;
		swap_chain->GetFullscreenState(&fullscreen, nullptr);
		swap_chain->SetFullscreenState(FALSE, nullptr);
		swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, flags);
		swap_chain->SetFullscreenState(fullscreen, nullptr);

		// Recreate the RTV
		createRenderTargetView();
	}


private:
	void createSwapChain() {
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

		if (not display_config.isVsync()) {
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

	void createRenderTargetView() {
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


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	HWND window;
	DisplayConfig& display_config;

	ID3D11Device& device;
	ID3D11DeviceContext& device_context;

	ComPtr<IDXGISwapChain1> swap_chain;
	ComPtr<ID3D11RenderTargetView> render_target_view;
};

} //namespace render
