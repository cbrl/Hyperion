#include "output_mgr.h"
#include "directx/directxtk.h"


OutputMgr::OutputMgr(DisplayConfig& config,
                     ID3D11Device& device,
                     SwapChain& swap_chain)
	: display_config(config)
	, device(device)
	, swap_chain(swap_chain) {

	createDepthBuffer();
}


void OutputMgr::createDepthBuffer() {

	// Texture desc
	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width            = display_config.getDisplayDesc().Width;
	texture_desc.Height           = display_config.getDisplayDesc().Height;
	texture_desc.MipLevels        = 1;
	texture_desc.ArraySize        = 1;
	texture_desc.Format           = DXGI_FORMAT_R32_TYPELESS;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.Usage            = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

	// Validate multisample quality
	if (texture_desc.SampleDesc.Count != 1) {

		ThrowIfFailed(device.CheckMultisampleQualityLevels(texture_desc.Format,
		                                                   texture_desc.SampleDesc.Count,
		                                                   &texture_desc.SampleDesc.Quality),
					  "Failed to create multisampled Texture2D");
					  
		ThrowIfFailed((texture_desc.SampleDesc.Quality != 0),
					  "Failed to create multisampled Texture2D");

		--texture_desc.SampleDesc.Quality;
	}


	// Create the texture.
	ComPtr< ID3D11Texture2D > texture;
	ThrowIfFailed(device.CreateTexture2D(&texture_desc,
	                                     nullptr,
	                                     texture.ReleaseAndGetAddressOf()),
	              "Failed to create Texture2D");


	// Create the DSV
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
	                                            depth_stencil_view.ReleaseAndGetAddressOf()),
	              "Failed to create DSV");

	SetDebugObjectName(depth_stencil_view.Get(), "Output Manager DepthStencilView");
}


void OutputMgr::bindBeginForward(ID3D11DeviceContext& device_context) const noexcept {
	Pipeline::OM::clearDSV(device_context, depth_stencil_view.Get());
	Pipeline::OM::bindRTVAndDSV(device_context, swap_chain.getRTV(), depth_stencil_view.Get());
}


void OutputMgr::bindEndForward(ID3D11DeviceContext& device_context) const noexcept {
	Pipeline::OM::bindRTVAndDSV(device_context, nullptr, nullptr);
}

void OutputMgr::bindEnd(ID3D11DeviceContext& device_context) const noexcept {
	Pipeline::OM::bindRTVAndDSV(device_context, swap_chain.getRTV(), nullptr);
}
