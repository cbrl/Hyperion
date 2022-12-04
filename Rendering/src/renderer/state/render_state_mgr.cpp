module;

#include "directx/d3d11.h"
#include "directx/directxtk.h"
#include "hlsl.h"

module rendering;

import :pipeline;
import :render_states;


namespace render {

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
	bind(device_context, SamplerStates::PointClamp, SLOT_SAMPLER_POINT_CLAMP);
	bind(device_context, SamplerStates::PointWrap, SLOT_SAMPLER_POINT_WRAP);
	bind(device_context, SamplerStates::LinearClamp, SLOT_SAMPLER_LINEAR_CLAMP);
	bind(device_context, SamplerStates::LinearWrap, SLOT_SAMPLER_LINEAR_WRAP);
	bind(device_context, SamplerStates::AnisotropicClamp, SLOT_SAMPLER_ANISO_CLAMP);
	bind(device_context, SamplerStates::AnisotropicWrap, SLOT_SAMPLER_ANISO_WRAP);
	bind(device_context, SamplerStates::PCF, SLOT_SAMPLER_PCF);
}


HRESULT RenderStateMgr::createBlendState(ID3D11Device& device,
                                         D3D11_BLEND src_blend,
                                         D3D11_BLEND dest_blend,
                                         gsl::not_null<ID3D11BlendState**> p_result) const {
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
                                                gsl::not_null<ID3D11DepthStencilState**> p_result) const {
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
                                              gsl::not_null<ID3D11RasterizerState**> p_result) const {
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
                                           gsl::not_null<ID3D11SamplerState**> p_result) const {
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
	ThrowIfFailed(createBlendState(device,
	                               D3D11_BLEND_ONE,
	                               D3D11_BLEND_ZERO,
	                               getAddressOf(BlendStates::Opaque)),
	              "Error creating Opaque blend state");

	// Alpha Blend
	ThrowIfFailed(createBlendState(device,
	                               D3D11_BLEND_ONE,
	                               D3D11_BLEND_INV_SRC_ALPHA,
	                               getAddressOf(BlendStates::AlphaBlend)),
	              "Error creating AlphaBlend blend state");

	// Additive
	ThrowIfFailed(createBlendState(device,
	                               D3D11_BLEND_SRC_ALPHA,
	                               D3D11_BLEND_ONE,
	                               getAddressOf(BlendStates::Additive)),
	              "Error creating Additive blend state");

	// Non-Premultiplied
	ThrowIfFailed(createBlendState(device,
	                               D3D11_BLEND_SRC_ALPHA,
	                               D3D11_BLEND_INV_SRC_ALPHA,
	                               getAddressOf(BlendStates::NonPremultiplied)),
	              "Error creating NonPremultiplied blend state");
}


void RenderStateMgr::createDepthStencilStates(ID3D11Device& device) {

	// None
	ThrowIfFailed(createDepthStencilState(device,
	                                      false,
	                                      false,
	                                      false,
	                                      getAddressOf(DepthStencilStates::None)),
	              "Error creating None depth stencil state");

	// Less Equal R
	ThrowIfFailed(createDepthStencilState(device,
	                                      true,
	                                      false,
	                                      false,
	                                      getAddressOf(DepthStencilStates::LessEqR)),
				  "Error creating LessEqR depth stencil state");

	// Less Equal R/W
	ThrowIfFailed(createDepthStencilState(device,
	                                      true,
	                                      true,
	                                      false,
	                                      getAddressOf(DepthStencilStates::LessEqRW)),
	              "Error creating LessEqRW depth stencil state");

	// Greater Equal R
	ThrowIfFailed(createDepthStencilState(device,
	                                      true,
	                                      false,
	                                      true,
	                                      getAddressOf(DepthStencilStates::GreaterEqR)),
				  "Error creating GreaterEqR depth stencil state");

	// Greater Equal R/W
	ThrowIfFailed(createDepthStencilState(device,
	                                      true,
	                                      true,
	                                      true,
	                                      getAddressOf(DepthStencilStates::GreaterEqRW)),
				  "Error creating GreaterEqRW depth stencil state");
}


void RenderStateMgr::createRasterizerStates(ID3D11Device& device) {

	// Cull None
	ThrowIfFailed(createRasterizerState(device,
	                                    D3D11_CULL_NONE,
	                                    D3D11_FILL_SOLID,
	                                    getAddressOf(RasterStates::CullNone)),
	              "Error creating CullNone raster state");

	// Cull Clockwise
	ThrowIfFailed(createRasterizerState(device,
	                                    D3D11_CULL_FRONT,
	                                    D3D11_FILL_SOLID,
	                                    getAddressOf(RasterStates::CullClockwise)),
	              "Error creating CullClockwise raster state");

	// Cull Counterclockwise
	ThrowIfFailed(createRasterizerState(device,
	                                    D3D11_CULL_BACK,
	                                    D3D11_FILL_SOLID,
	                                    getAddressOf(RasterStates::CullCounterClockwise)),
	              "Error creating CullCounterClockwise raster state");

	// Wireframe
	ThrowIfFailed(createRasterizerState(device,
	                                    D3D11_CULL_NONE,
	                                    D3D11_FILL_WIREFRAME,
	                                    getAddressOf(RasterStates::Wireframe)),
	              "Error creating Wireframe raster state");
}


void RenderStateMgr::createSamplerStates(ID3D11Device& device) {

	// Point Wrap
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_POINT,
	                                 D3D11_TEXTURE_ADDRESS_WRAP,
	                                 getAddressOf(SamplerStates::PointWrap)),
	              "Error creating PointWrap sampler state");

	// Point Clamp
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_POINT,
	                                 D3D11_TEXTURE_ADDRESS_CLAMP,
	                                 getAddressOf(SamplerStates::PointClamp)),
	              "Error creating PointClamp sampler state");

	// Linear Wrap
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	                                 D3D11_TEXTURE_ADDRESS_WRAP,
	                                 getAddressOf(SamplerStates::LinearWrap)),
	              "Error creating LinearWrap sampler state");

	// Linear Clamp
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	                                 D3D11_TEXTURE_ADDRESS_CLAMP,
	                                 getAddressOf(SamplerStates::LinearClamp)),
	              "Error creating LinearClamp sampler state");

	// Anisotropic Wrap
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_ANISOTROPIC,
	                                 D3D11_TEXTURE_ADDRESS_WRAP,
	                                 getAddressOf(SamplerStates::AnisotropicWrap)),
	              "Error creating AnisotropicWrap sampler state");

	// Anisotropic Clamp
	ThrowIfFailed(createSamplerState(device,
	                                 D3D11_FILTER_ANISOTROPIC,
	                                 D3D11_TEXTURE_ADDRESS_CLAMP,
	                                 getAddressOf(SamplerStates::AnisotropicClamp)),
	              "Error creating AnisotropicClamp sampler state");

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

		ThrowIfFailed(device.CreateSamplerState(&desc, getAddressOf(SamplerStates::PCF)),
		              "Error creating pcf sampler state");

		SetDebugObjectName(get(SamplerStates::PCF), "RenderStateMgr SamplerState");
	}
}


void RenderStateMgr::bind(ID3D11DeviceContext& device_context,
                          BlendStates state,
                          f32 blend_factor[4],
                          u32 sample_mask) const {
	
	Pipeline::OM::bindBlendState(device_context, get(state), blend_factor, sample_mask);
}

void RenderStateMgr::bind(ID3D11DeviceContext& device_context, DepthStencilStates state, u32 stencil_ref) const {
	
	Pipeline::OM::bindDepthStencilState(device_context, get(state), stencil_ref);
}

void RenderStateMgr::bind(ID3D11DeviceContext& device_context, RasterStates state) const {
	
	Pipeline::RS::bindState(device_context, get(state));
}

void RenderStateMgr::bind(ID3D11DeviceContext& device_context, SamplerStates state, u32 slot) const {
	
	Pipeline::bindSampler(device_context, slot, get(state));
}

} //namespace render