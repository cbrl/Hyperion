#pragma once

#include "directx/d3d11.h"
#include "direct3d/swapchain.h"
#include "display/display_config.h"


class OutputMgr final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------

	OutputMgr(DisplayConfig& config,
	          ID3D11Device& device,
	          SwapChain& swap_chain);

	OutputMgr(const OutputMgr& mgr) = delete;
	OutputMgr(OutputMgr&& mgr) noexcept = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------

	~OutputMgr() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------

	OutputMgr& operator=(const OutputMgr& mgr) = delete;
	OutputMgr& operator=(OutputMgr&& mgr) = delete;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void resizeBuffers() {
		createDepthBuffer();
	}

	void bindBegin(ID3D11DeviceContext& device_context) const noexcept;
	void bindEnd(ID3D11DeviceContext& device_context) const noexcept;

	void bindBeginForward(ID3D11DeviceContext& device_context) const noexcept;
	void bindEndForward(ID3D11DeviceContext& device_context) const noexcept;


private:
	void createDepthBuffer();


private:
	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	DisplayConfig& display_config;
	ID3D11Device& device;
	SwapChain& swap_chain;

	ComPtr<ID3D11DepthStencilView> depth_stencil_view;
};
