#pragma once

#include <d3d11.h>


class RenderStateMgr final {
	public:
		RenderStateMgr(ID3D11Device& device, ID3D11DeviceContext& device_context);
		~RenderStateMgr();

		void SetupStates(ID3D11Device& device, ID3D11DeviceContext& device_context);


		//----------------------------------------------------------------------------------
		// Bind states
		//----------------------------------------------------------------------------------

		// Blend states
		void BindOpaque(ID3D11DeviceContext& device_context, float blendFactor[4] = {}, u32 sampleMask = 0xffffffff) const;
		void BindAlphaBlend(ID3D11DeviceContext& device_context, float blendFactor[4] = {}, u32 sampleMask = 0xffffffff) const;
		void BindAdditive(ID3D11DeviceContext& device_context, float blendFactor[4] = {}, u32 sampleMask = 0xffffffff) const;
		void BindNonPremultiplied(ID3D11DeviceContext& device_context, float blendFactor[4] = {}, u32 sampleMask = 0xffffffff) const;

		// Depth stencil states
		void BindDepthNone(ID3D11DeviceContext& device_context, u32 stencilRef = 0) const;
		void BindDepthDefault(ID3D11DeviceContext& device_context, u32 stencilRef = 0) const;
		void BindDepthRead(ID3D11DeviceContext& device_context, u32 stencilRef = 0) const;

		// Rasterizer states
		void BindCullNone(ID3D11DeviceContext& device_context) const;
		void BindCullClockwise(ID3D11DeviceContext& device_context) const;
		void BindCullCounterClockwise(ID3D11DeviceContext& device_context) const;
		void BindWireframe(ID3D11DeviceContext& device_context) const;

		// Sampler states
		void BindPointWrap(ID3D11DeviceContext& device_context) const;
		void BindPointClamp(ID3D11DeviceContext& device_context) const;
		void BindLinearWrap(ID3D11DeviceContext& device_context) const;
		void BindLinearClamp(ID3D11DeviceContext& device_context) const;
		void BindAnisotropicWrap(ID3D11DeviceContext& device_context) const;
		void BindAnisotropicClamp(ID3D11DeviceContext& device_context) const;
		void BindPCFSampler(ID3D11DeviceContext& device_context) const;


	private:
		//----------------------------------------------------------------------------------
		// Create states
		//----------------------------------------------------------------------------------

		// Create a render states
		HRESULT CreateBlendState(ID3D11Device& device, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, _Out_ ID3D11BlendState** pResult);
		HRESULT CreateDepthStencilState(ID3D11Device& device, bool enable, bool writeEnable, _Out_ ID3D11DepthStencilState** pResult);
		HRESULT CreateRasterizerState(ID3D11Device& device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, _Out_ ID3D11RasterizerState** pResult);
		HRESULT CreateSamplerState(ID3D11Device& device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, _Out_ ID3D11SamplerState** pResult);

		// Create the standard render states
		void CreateBlendStates(ID3D11Device& device);
		void CreateDepthStencilStates(ID3D11Device& device);
		void CreateRasterizerStates(ID3D11Device& device);
		void CreateSamplerStates(ID3D11Device& device);


	private:
		// Blend states
		ComPtr<ID3D11BlendState> opaque;
		ComPtr<ID3D11BlendState> alpha_blend;
		ComPtr<ID3D11BlendState> additive;
		ComPtr<ID3D11BlendState> non_premultiplied;

		// Depth stencil states
		ComPtr<ID3D11DepthStencilState> depth_none;
		ComPtr<ID3D11DepthStencilState> depth_default;
		ComPtr<ID3D11DepthStencilState> depth_read;

		// Rasterizer states
		ComPtr<ID3D11RasterizerState> cull_none;
		ComPtr<ID3D11RasterizerState> cull_clockwise;
		ComPtr<ID3D11RasterizerState> cull_counter_clockwise;
		ComPtr<ID3D11RasterizerState> wireframe;

		// Sampler states
		ComPtr<ID3D11SamplerState> point_wrap;
		ComPtr<ID3D11SamplerState> point_clamp;
		ComPtr<ID3D11SamplerState> linear_wrap;
		ComPtr<ID3D11SamplerState> linear_clamp;
		ComPtr<ID3D11SamplerState> anisotropic_wrap;
		ComPtr<ID3D11SamplerState> anisotropic_clamp;
		ComPtr<ID3D11SamplerState> pcf_sampler;
};