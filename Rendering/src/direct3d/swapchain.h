#pragma once

#include "directx/d3d11.h"
#include "display/display_config.h"


class SwapChain final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	SwapChain(HWND window,
	          DisplayConfig& config,
	          ID3D11Device& device,
	          ID3D11DeviceContext& device_context);

	SwapChain(const SwapChain& swap_chain) = delete;
	SwapChain(SwapChain&& swap_chain) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~SwapChain();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	SwapChain& operator=(const SwapChain& swap_chain) = delete;
	SwapChain& operator=(SwapChain&& swap_chain) = delete;


	//----------------------------------------------------------------------------------
	// Member Functions - Render Target View
	//----------------------------------------------------------------------------------

	// Clear the render target view
	void clear() const noexcept;
	void clear(const f32 color[4]) const noexcept;

	// Get a pointer to the render target view
	ID3D11RenderTargetView* getRTV() const noexcept {
		return render_target_view.Get();
	}


	//----------------------------------------------------------------------------------
	// Member Functions - Frame
	//----------------------------------------------------------------------------------

	// Present the frame
	void present() const;


	//----------------------------------------------------------------------------------
	// Member Functions - Fullscreen
	//----------------------------------------------------------------------------------

	// Switch the fullscreen state. toggle_state determines if the
	// swap chain's fullscreen state also needs to be changed.
	void switchMode(bool toggle_state);

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
	void reset();


private:
	void createSwapChain();
	void createRenderTargetView();


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