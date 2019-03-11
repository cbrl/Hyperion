#include "output_mgr.h"
#include "directx/directxtk.h"
#include "hlsl.h"


OutputMgr::OutputMgr(DisplayConfig& config,
                     ID3D11Device& device,
                     SwapChain& swap_chain)
	: display_config(config)
	, device(device)
	, swap_chain(swap_chain) {

	const auto resolution = display_config.getDisplayResolution();

	//----------------------------------------------------------------------------------
	// Depth Buffer
	//----------------------------------------------------------------------------------
	createDepthBuffer(resolution, 1);

	//----------------------------------------------------------------------------------
	// Base Color GBuffer
	//----------------------------------------------------------------------------------
	createBuffer(resolution,
	             1,
	             DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	             releaseAndGetAddressOf(RTV::GBufferColor),
	             releaseAndGetAddressOf(SRV::GBufferColor));

	SetDebugObjectName(get(RTV::GBufferColor), "Output Manager GBuffer Color RTV");
	SetDebugObjectName(get(SRV::GBufferColor), "Output Manager GBuffer Color SRV");

	//----------------------------------------------------------------------------------
	// Material GBuffer
	//----------------------------------------------------------------------------------
	createBuffer(resolution,
	             1,
	             DXGI_FORMAT_R8G8_UNORM,
	             releaseAndGetAddressOf(RTV::GBufferMaterial),
	             releaseAndGetAddressOf(SRV::GBufferMaterial));

	SetDebugObjectName(get(RTV::GBufferMaterial), "Output Manager GBuffer Material RTV");
	SetDebugObjectName(get(SRV::GBufferMaterial), "Output Manager GBuffer Material SRV");

	//----------------------------------------------------------------------------------
	// Normal GBuffer
	//----------------------------------------------------------------------------------
	createBuffer(resolution,
	             1,
	             DXGI_FORMAT_R32G32B32A32_FLOAT,
	             releaseAndGetAddressOf(RTV::GBufferNormal),
	             releaseAndGetAddressOf(SRV::GBufferNormal));

	SetDebugObjectName(get(RTV::GBufferNormal), "Output Manager GBuffer Normal RTV");
	SetDebugObjectName(get(SRV::GBufferNormal), "Output Manager GBuffer Normal SRV");
}


ID3D11RenderTargetView* OutputMgr::get(RTV rtv) const noexcept {
	return rtvs[static_cast<size_t>(rtv)].Get();
}


ID3D11ShaderResourceView* OutputMgr::get(SRV srv) const noexcept {
	return srvs[static_cast<size_t>(srv)].Get();
}


gsl::not_null<ID3D11RenderTargetView**> OutputMgr::releaseAndGetAddressOf(RTV rtv) {
	auto** pptr = rtvs[static_cast<size_t>(rtv)].ReleaseAndGetAddressOf();
	return gsl::make_not_null(pptr);
}


gsl::not_null<ID3D11ShaderResourceView**> OutputMgr::releaseAndGetAddressOf(SRV srv) {
	auto** pptr = srvs[static_cast<size_t>(srv)].ReleaseAndGetAddressOf();
	return gsl::make_not_null(pptr);
}


void OutputMgr::createBuffer(u32_2 resolution,
                             u32 samples,
                             DXGI_FORMAT format,
                             ID3D11RenderTargetView** rtv,
                             ID3D11ShaderResourceView** srv) {

	//----------------------------------------------------------------------------------
	// Texture
	//----------------------------------------------------------------------------------
	ComPtr<ID3D11Texture2D> texture;

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width            = resolution[0];
	texture_desc.Height           = resolution[1];
	texture_desc.MipLevels        = 1;
	texture_desc.ArraySize        = 1;
	texture_desc.Format           = format;
	texture_desc.SampleDesc.Count = samples;
	texture_desc.Usage            = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

	if (rtv) {
		texture_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	// Validate multisample quality
	if (texture_desc.SampleDesc.Count != 1) {
		const auto result = device.CheckMultisampleQualityLevels(texture_desc.Format,
		                                                         texture_desc.SampleDesc.Count,
		                                                         &texture_desc.SampleDesc.Quality);

		ThrowIfFailed(result, "Failed to create multisampled Texture2D");

		ThrowIfFailed(texture_desc.SampleDesc.Quality != 0,
		              "Failed to create multisampled Texture2D");

		--texture_desc.SampleDesc.Quality;
	}

	ThrowIfFailed(device.CreateTexture2D(&texture_desc,
	                                     nullptr,
	                                     texture.ReleaseAndGetAddressOf()),
	              "Failed to create Texture2D");

	//----------------------------------------------------------------------------------
	// SRV
	//----------------------------------------------------------------------------------
	ThrowIfFailed(device.CreateShaderResourceView(texture.Get(), nullptr, srv),
	              "Failed to create Shader Resource View");

	//----------------------------------------------------------------------------------
	// RTV
	//----------------------------------------------------------------------------------
	if (rtv) {
		ThrowIfFailed(device.CreateRenderTargetView(texture.Get(), nullptr, rtv),
		              "Failed to create Render Target View");
	}
}


void OutputMgr::createDepthBuffer(u32_2 resolution, u32 samples) {

	//----------------------------------------------------------------------------------
	// Texture
	//----------------------------------------------------------------------------------
	ComPtr<ID3D11Texture2D> texture;

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width            = resolution[0];
	texture_desc.Height           = resolution[1];
	texture_desc.MipLevels        = 1;
	texture_desc.ArraySize        = 1;
	texture_desc.Format           = DXGI_FORMAT_R32_TYPELESS;
	texture_desc.SampleDesc.Count = samples;
	texture_desc.Usage            = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

	// Validate multisample quality
	if (texture_desc.SampleDesc.Count != 1) {
		const auto result = device.CheckMultisampleQualityLevels(texture_desc.Format,
		                                                         texture_desc.SampleDesc.Count,
		                                                         &texture_desc.SampleDesc.Quality);

		ThrowIfFailed(result, "Failed to create multisampled Texture2D");
					  
		ThrowIfFailed(texture_desc.SampleDesc.Quality != 0,
					  "Failed to create multisampled Texture2D");

		--texture_desc.SampleDesc.Quality;
	}

	ThrowIfFailed(device.CreateTexture2D(&texture_desc,
	                                     nullptr,
	                                     texture.ReleaseAndGetAddressOf()),
	              "Failed to create Texture2D");

	//----------------------------------------------------------------------------------
	// Shader Resource View
	//----------------------------------------------------------------------------------
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	if (texture_desc.SampleDesc.Count == 1) {
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = 1;
	}
	else {
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}

	ThrowIfFailed(device.CreateShaderResourceView(texture.Get(),
	                                              &srv_desc,
	                                              releaseAndGetAddressOf(SRV::GBufferDepth)),
	              "Failed to create Shader Rsource View");

	SetDebugObjectName(get(SRV::GBufferDepth), "Output Manager GBuffer Depth SRV");

	//----------------------------------------------------------------------------------
	// Depth Stencil View
	//----------------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;

	if (texture_desc.SampleDesc.Count == 1) {
		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	}
	else {
		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}

	ThrowIfFailed(device.CreateDepthStencilView(texture.Get(),
	                                            &dsv_desc,
	                                            dsv.ReleaseAndGetAddressOf()),
	              "Failed to create Depth Stencil View");

	SetDebugObjectName(dsv.Get(), "Output Manager DepthStencilView");
}


void OutputMgr::bindBegin(ID3D11DeviceContext& device_context) const {
	Pipeline::OM::clearDSV(device_context, dsv.Get());
}


void OutputMgr::bindEnd(ID3D11DeviceContext& device_context) const {
	Pipeline::OM::bindRTVAndDSV(device_context, swap_chain.getRTV(), nullptr);
}


void OutputMgr::bindBeginForward(ID3D11DeviceContext& device_context) const {
	Pipeline::OM::bindRTVAndDSV(device_context, swap_chain.getRTV(), dsv.Get());
}


void OutputMgr::bindEndForward(ID3D11DeviceContext& device_context) const {
	Pipeline::OM::bindRTVAndDSV(device_context, nullptr, nullptr);
}


void OutputMgr::bindBeginDeferred(ID3D11DeviceContext& device_context) const {
	static_assert(SLOT_SRV_MATERIAL_PARAMS == SLOT_SRV_BASE_COLOR + 1);
	static_assert(SLOT_SRV_NORMAL          == SLOT_SRV_MATERIAL_PARAMS + 1);
	static_assert(SLOT_SRV_DEPTH           == SLOT_SRV_NORMAL + 1);

	ID3D11ShaderResourceView* const srvs[] = {
	    get(SRV::GBufferColor),
	    get(SRV::GBufferMaterial),
	    get(SRV::GBufferNormal),
	    get(SRV::GBufferDepth)
	};

	Pipeline::PS::bindSRVs(device_context, SLOT_SRV_BASE_COLOR, gsl::span{srvs});
	Pipeline::OM::bindRTVAndDSV(device_context, swap_chain.getRTV(), nullptr);
}


void OutputMgr::bindEndDeferred(ID3D11DeviceContext& device_context) const {
	ID3D11ShaderResourceView* const srvs[4] = { nullptr };

	Pipeline::PS::bindSRVs(device_context, SLOT_SRV_BASE_COLOR, gsl::span{srvs});
	Pipeline::OM::bindRTVAndDSV(device_context, nullptr, nullptr);
}


void OutputMgr::bindBeginGBuffer(ID3D11DeviceContext& device_context) const {
	ID3D11RenderTargetView* const rtvs[] = {
	    get(RTV::GBufferColor),
	    get(RTV::GBufferMaterial),
	    get(RTV::GBufferNormal)
	};

	Pipeline::OM::bindRTVsAndDSV(device_context, gsl::span{rtvs}, dsv.Get());
}


void OutputMgr::bindEndGBuffer(ID3D11DeviceContext& device_context) const {
	Pipeline::OM::bindRTVAndDSV(device_context, nullptr, nullptr);
}