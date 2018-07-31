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

	~SwapChain() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	SwapChain& operator=(const SwapChain& swap_chain) = delete;
	SwapChain& operator=(SwapChain&& swap_chain) = delete;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Clear the render target view
	void clear() const noexcept;
	void clear(const f32 color[4]) const noexcept;

	// Present the frame
	void present() const;

	// Set the fullscreen state
	void setFullscreen(bool state);

	// Get a pointer to the render target view
	ID3D11RenderTargetView* getRTV() const {
		return render_target_view.Get();
	}


private:
	void createSwapChain();
	void createRenderTargetView();
	void resetSwapChain();


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