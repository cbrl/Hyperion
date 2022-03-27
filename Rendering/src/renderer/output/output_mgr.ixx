module;

#include "directx/d3d11.h"

export module rendering.output_mgr;

import rendering.display_config;
import rendering.swapchain;


export namespace render {

class OutputMgr final {
private:
	//----------------------------------------------------------------------------------
	// Enums
	//----------------------------------------------------------------------------------

	enum class RTV : u8 {
		GBufferColor = 0,
		GBufferMaterial,
		GBufferNormal,
		Size
	};

	enum class SRV : u8 {
		GBufferColor = 0,
		GBufferMaterial,
		GBufferNormal,
		GBufferDepth,
		Size
	};

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
		createDepthBuffer(display_config.getDisplayResolution(), 1);
	}

	void bindBegin(ID3D11DeviceContext& device_context) const;
	void bindEnd(ID3D11DeviceContext& device_context) const;

	void bindBeginForward(ID3D11DeviceContext& device_context) const;
	void bindEndForward(ID3D11DeviceContext& device_context) const;

	void bindBeginDeferred(ID3D11DeviceContext& device_context) const;
	void bindEndDeferred(ID3D11DeviceContext& device_context) const;

	void bindBeginGBuffer(ID3D11DeviceContext& device_context) const;
	void bindEndGBuffer(ID3D11DeviceContext& device_context) const;

private:

	void createBuffer(u32_2 resolution,
	                  u32 samples,
	                  DXGI_FORMAT format,
	                  ID3D11RenderTargetView** rtv,
	                  ID3D11ShaderResourceView** srv);

	void createDepthBuffer(u32_2 resolution, u32 samples);

	ID3D11RenderTargetView*   get(RTV rtv) const noexcept;
	ID3D11ShaderResourceView* get(SRV srv) const noexcept;

	gsl::not_null<ID3D11RenderTargetView**>   releaseAndGetAddressOf(RTV rtv);
	gsl::not_null<ID3D11ShaderResourceView**> releaseAndGetAddressOf(SRV srv);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Dependency References
	DisplayConfig& display_config;
	ID3D11Device&  device;
	SwapChain&     swap_chain;

	// Depth Stencil View
	ComPtr<ID3D11DepthStencilView> dsv;

	// Render Target Views
	std::array<ComPtr<ID3D11RenderTargetView>, static_cast<size_t>(RTV::Size)> rtvs;

	// Shader Resource Views
	std::array<ComPtr<ID3D11ShaderResourceView>, static_cast<size_t>(SRV::Size)> srvs;
};

} //namespace render
