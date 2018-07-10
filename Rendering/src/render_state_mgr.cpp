#include "render_state_mgr.h"

#include "directx/directxtk.h"
#include "hlsl.h"


RenderStateMgr::RenderStateMgr(ID3D11Device& device, ID3D11DeviceContext& device_context) {
	setupStates(device, device_context);
}


void RenderStateMgr::setupStates(ID3D11Device& device, ID3D11DeviceContext& device_context) {
	// Blend states
	createBlendStates(device);

	// Depth stencil states
	createDepthStencilStates(device);

	// Rasterizer states
	createRasterizerStates(device);

	// Sampler states
	createSamplerStates(device);

	// Bind Sampler states
	bindLinearClamp(device_context);
	bindLinearWrap(device_context);
	bindPointClamp(device_context);
	bindPointWrap(device_context);
	bindAnisotropicClamp(device_context);
	bindAnisotropicWrap(device_context);
	bindPcfSampler(device_context);
}


HRESULT RenderStateMgr::createBlendState(ID3D11Device& device,
                                         D3D11_BLEND src_blend,
                                         D3D11_BLEND dest_blend,
                                         ID3D11BlendState** p_result) const {
	D3D11_BLEND_DESC desc = {};

	desc.RenderTarget[0].BlendEnable = (src_blend != D3D11_BLEND_ONE) || (dest_blend != D3D11_BLEND_ZERO);

	desc.RenderTarget[0].SrcBlend  = desc.RenderTarget[0].SrcBlendAlpha  = src_blend;
	desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = dest_blend;
	desc.RenderTarget[0].BlendOp   = desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	const HRESULT hr = device.CreateBlendState(&desc, p_result);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*p_result, "RenderStateMgr BlendState");
	}

	return hr;
}


HRESULT RenderStateMgr::createDepthStencilState(ID3D11Device& device,
                                                bool enable,
                                                bool write_enable,
                                                bool depth_greater,
                                                ID3D11DepthStencilState** p_result) const {
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable    = enable ? TRUE : FALSE;
	desc.DepthWriteMask = write_enable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc      = depth_greater ? D3D11_COMPARISON_GREATER_EQUAL : D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable    = FALSE;
	desc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	desc.BackFace = desc.FrontFace;

	const HRESULT hr = device.CreateDepthStencilState(&desc, p_result);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*p_result, "RenderStateMgr DepthState");
	}

	return hr;
}


HRESULT RenderStateMgr::createRasterizerState(ID3D11Device& device,
                                              D3D11_CULL_MODE cull_mode,
                                              D3D11_FILL_MODE fill_mode,
                                              ID3D11RasterizerState** p_result) const {
	D3D11_RASTERIZER_DESC desc = {};

	desc.CullMode          = cull_mode;
	desc.FillMode          = fill_mode;
	desc.DepthClipEnable   = TRUE;
	desc.MultisampleEnable = TRUE;

	const HRESULT hr = device.CreateRasterizerState(&desc, p_result);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*p_result, "RenderStateMgr RasterState");
	}

	return hr;
}


HRESULT RenderStateMgr::createSamplerState(ID3D11Device& device,
                                           D3D11_FILTER filter,
                                           D3D11_TEXTURE_ADDRESS_MODE address_mode,
                                           ID3D11SamplerState** p_result) const {
	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = filter;

	desc.AddressU = address_mode;
	desc.AddressV = address_mode;
	desc.AddressW = address_mode;

	desc.MaxAnisotropy = (device.GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2;

	desc.MaxLOD         = D3D11_FLOAT32_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	const HRESULT hr = device.CreateSamplerState(&desc, p_result);

	if (SUCCEEDED(hr)) {
		SetDebugObjectName(*p_result, "RenderStateMgr SamplerState");
	}

	return hr;
}


void RenderStateMgr::createBlendStates(ID3D11Device& device) {
	// Opaque
	ThrowIfFailed(createBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, opaque.GetAddressOf()),
	              "Error creating opaque blend state");

	// Alpha Blend
	ThrowIfFailed(createBlendState(device, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, alpha_blend.GetAddressOf()),
	              "Error creating alphablend blend state");

	// Additive
	ThrowIfFailed(createBlendState(device, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, additive.GetAddressOf()),
	              "Error creating additive blend state");

	// Non-Premultiplied
	ThrowIfFailed(createBlendState(device,
	                               D3D11_BLEND_SRC_ALPHA,
	                               D3D11_BLEND_INV_SRC_ALPHA,
	                               non_premultiplied.GetAddressOf()),
	              "Error creating nonpremultiplied blend state");
}


void RenderStateMgr::createDepthStencilStates(ID3D11Device& device) {
	// Depth None
	ThrowIfFailed(createDepthStencilState(device, false, false, false, depth_none.GetAddressOf()),
	              "Error creating depth stencil state");

	// Depth Less/Equal Read/Write
	ThrowIfFailed(createDepthStencilState(device, true, true, false, depth_less_equal_readwrite.GetAddressOf()),
	              "Error creating depth stencil state");

	// Depth Greater/Equal Read/Write
	ThrowIfFailed(createDepthStencilState(device, true, true, true, depth_greater_equal_readwrite.GetAddressOf()),
				  "Error creating depth stencil state");

	// Depth Less/Equal Read
	ThrowIfFailed(createDepthStencilState(device, true, false, false, depth_less_equal_read.GetAddressOf()),
	              "Error creating depth stencil state");
}


void RenderStateMgr::createRasterizerStates(ID3D11Device& device) {
	// Cull None
	ThrowIfFailed(createRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_SOLID, cull_none.GetAddressOf()),
	              "Error creating cullnone raster state");

	// Cull Clockwise
	ThrowIfFailed(createRasterizerState(device, D3D11_CULL_FRONT, D3D11_FILL_SOLID, cull_clockwise.GetAddressOf()),
	              "Error creating cullclockwise raster state");

	// Cull Counterclockwise
	ThrowIfFailed(createRasterizerState(device,
	                                    D3D11_CULL_BACK,
	                                    D3D11_FILL_SOLID,
	                                    cull_counter_clockwise.GetAddressOf()),
	              "Error creating cullcounterclockwise raster state");

	// Wireframe
	ThrowIfFailed(createRasterizerState(device, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, wireframe.GetAddressOf()),
	              "Error creating wireframe raster state");
}


void RenderStateMgr::createSamplerStates(ID3D11Device& device) {
	// Point Wrap
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_POINT,
	                                 D3D11_TEXTURE_ADDRESS_WRAP,
	                                 point_wrap.GetAddressOf()),
	              "Error creating pointwrap sampler state");

	// Point Clamp
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_POINT,
	                                 D3D11_TEXTURE_ADDRESS_CLAMP,
	                                 point_clamp.GetAddressOf()),
	              "Error creating pointclamp sampler state");

	// Linear Wrap
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	                                 D3D11_TEXTURE_ADDRESS_WRAP,
	                                 linear_wrap.GetAddressOf()),
	              "Error creating linearwrap sampler state");

	// Linear Clamp
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	                                 D3D11_TEXTURE_ADDRESS_CLAMP,
	                                 linear_clamp.GetAddressOf()),
	              "Error creating linearclamp sampler state");

	// Anisotropic Wrap
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_ANISOTROPIC,
	                                 D3D11_TEXTURE_ADDRESS_WRAP,
	                                 anisotropic_wrap.GetAddressOf()),
	              "Error creating anisotropicwrap sampler state");

	// Anisotropic Clamp
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_ANISOTROPIC,
	                                 D3D11_TEXTURE_ADDRESS_CLAMP,
	                                 anisotropic_clamp.GetAddressOf()),
	              "Error creating anisotropicclamp sampler state");

	// PCF Sampler
	{
		D3D11_SAMPLER_DESC desc = {};

		desc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		desc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.MaxAnisotropy  = (device.GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2;
		desc.MaxLOD         = D3D11_FLOAT32_MAX;
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

		ThrowIfFailed(device.CreateSamplerState(&desc, pcf_sampler.GetAddressOf()),
		              "Error creating pcf sampler state");

		SetDebugObjectName(pcf_sampler.Get(), "RenderStateMgr SamplerState");
	}
}


//----------------------------------------------------------------------------------
// Bind blend states
//----------------------------------------------------------------------------------
void RenderStateMgr::bindOpaque(ID3D11DeviceContext& device_context, f32 blend_factor[4], u32 sample_mask) const {
	Pipeline::OM::bindBlendState(device_context, opaque.Get(), blend_factor, sample_mask);
}


void RenderStateMgr::bindAlphaBlend(ID3D11DeviceContext& device_context, f32 blend_factor[4], u32 sample_mask) const {
	Pipeline::OM::bindBlendState(device_context, alpha_blend.Get(), blend_factor, sample_mask);
}


void RenderStateMgr::bindAdditive(ID3D11DeviceContext& device_context, f32 blend_factor[4], u32 sample_mask) const {
	Pipeline::OM::bindBlendState(device_context, additive.Get(), blend_factor, sample_mask);
}


void RenderStateMgr::bindNonPremultiplied(ID3D11DeviceContext& device_context, f32 blend_factor[4], u32 sample_mask) const {
	Pipeline::OM::bindBlendState(device_context, non_premultiplied.Get(), blend_factor, sample_mask);
}


//----------------------------------------------------------------------------------
// Bind depth stencil states
//----------------------------------------------------------------------------------
void RenderStateMgr::bindDepthNone(ID3D11DeviceContext& device_context, u32 stencil_ref) const {
	Pipeline::OM::bindDepthStencilState(device_context, depth_none.Get(), stencil_ref);
}


void RenderStateMgr::bindDepthLessEqRW(ID3D11DeviceContext& device_context, u32 stencil_ref) const {
	Pipeline::OM::bindDepthStencilState(device_context, depth_less_equal_readwrite.Get(), stencil_ref);
}


void RenderStateMgr::bindDepthGreaterEqRW(ID3D11DeviceContext& device_context, u32 stencil_ref) const {
	Pipeline::OM::bindDepthStencilState(device_context, depth_greater_equal_readwrite.Get(), stencil_ref);
}


void RenderStateMgr::bindDepthLessEqRead(ID3D11DeviceContext& device_context, u32 stencil_ref) const {
	Pipeline::OM::bindDepthStencilState(device_context, depth_less_equal_read.Get(), stencil_ref);
}


//----------------------------------------------------------------------------------
// Bind rasterizer states
//----------------------------------------------------------------------------------
void RenderStateMgr::bindCullNone(ID3D11DeviceContext& device_context) const {
	Pipeline::RS::bindState(device_context, cull_none.Get());
}


void RenderStateMgr::bindCullClockwise(ID3D11DeviceContext& device_context) const {
	Pipeline::RS::bindState(device_context, cull_clockwise.Get());
}


void RenderStateMgr::bindCullCounterClockwise(ID3D11DeviceContext& device_context) const {
	Pipeline::RS::bindState(device_context, cull_counter_clockwise.Get());
}


void RenderStateMgr::bindWireframe(ID3D11DeviceContext& device_context) const {
	Pipeline::RS::bindState(device_context, wireframe.Get());
}


//----------------------------------------------------------------------------------
// Bind sampler states
//----------------------------------------------------------------------------------
void RenderStateMgr::bindPointWrap(ID3D11DeviceContext& device_context) const {
	Pipeline::bindSamplers(device_context, SLOT_SAMPLER_POINT_WRAP, 1, point_wrap.GetAddressOf());
}


void RenderStateMgr::bindPointClamp(ID3D11DeviceContext& device_context) const {
	Pipeline::bindSamplers(device_context, SLOT_SAMPLER_POINT_CLAMP, 1, point_clamp.GetAddressOf());
}


void RenderStateMgr::bindLinearWrap(ID3D11DeviceContext& device_context) const {
	Pipeline::bindSamplers(device_context, SLOT_SAMPLER_LINEAR_WRAP, 1, linear_wrap.GetAddressOf());
}


void RenderStateMgr::bindLinearClamp(ID3D11DeviceContext& device_context) const {
	Pipeline::bindSamplers(device_context, SLOT_SAMPLER_LINEAR_CLAMP, 1, linear_clamp.GetAddressOf());
}


void RenderStateMgr::bindAnisotropicWrap(ID3D11DeviceContext& device_context) const {
	Pipeline::bindSamplers(device_context, SLOT_SAMPLER_ANISO_WRAP, 1, anisotropic_wrap.GetAddressOf());
}


void RenderStateMgr::bindAnisotropicClamp(ID3D11DeviceContext& device_context) const {
	Pipeline::bindSamplers(device_context, SLOT_SAMPLER_ANISO_CLAMP, 1, anisotropic_clamp.GetAddressOf());
}


void RenderStateMgr::bindPcfSampler(ID3D11DeviceContext& device_context) const {
	Pipeline::bindSamplers(device_context, SLOT_SAMPLER_PCF, 1, pcf_sampler.GetAddressOf());
}
