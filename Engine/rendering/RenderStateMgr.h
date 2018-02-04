#pragma once

#include <d3d11.h>
#include <DirectXHelpers.h>
#include <wrl\client.h>

#include "util\EngineUtil.h"
#include "shader\HlslDefines.h"

using Microsoft::WRL::ComPtr;

using namespace DirectX;

class RenderStateMgr {
	public:
		static const RenderStateMgr* Get();

		RenderStateMgr(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
		~RenderStateMgr();

		void SetupStates();

		//----------------------------------------------------------------------------------
		// Bind states
		//----------------------------------------------------------------------------------
		// Blend states
		void BindOpaque(float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;
		void BindAlphaBlend(float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;
		void BindAdditive(float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;
		void BindNonPremultiplied(float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;

		// Depth stencil states
		void BindDepthNone(unsigned int stencilRef = 0) const;
		void BindDepthDefault(unsigned int stencilRef = 0) const;
		void BindDepthRead(unsigned int stencilRef = 0) const;

		// Rasterizer states
		void BindCullNone() const;
		void BindCullClockwise() const;
		void BindCullCounterClockwise() const;
		void BindWireframe() const;

		// Sampler states
		void BindPointWrap() const;
		void BindPointClamp() const;
		void BindLinearWrap() const;
		void BindLinearClamp() const;
		void BindAnisotropicWrap() const;
		void BindAnisotropicClamp() const;


	private:
		//----------------------------------------------------------------------------------
		// Create states
		//----------------------------------------------------------------------------------
		HRESULT CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, _Out_ ID3D11BlendState** pResult);
		HRESULT CreateDepthStencilState(bool enable, bool writeEnable, _Out_ ID3D11DepthStencilState** pResult);
		HRESULT CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, _Out_ ID3D11RasterizerState** pResult);
		HRESULT CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, _Out_ ID3D11SamplerState** pResult);

		// Blend states
		void Opaque();
		void AlphaBlend();
		void Additive();
		void NonPremultiplied();

		// Depth stencil states
		void DepthNone();
		void DepthDefault();
		void DepthRead();

		// Rasterizer states
		void CullNone();
		void CullClockwise();
		void CullCounterClockwise();
		void Wireframe();

		// Sampler states
		void PointWrap();
		void PointClamp();
		void LinearWrap();
		void LinearClamp();
		void AnisotropicWrap();
		void AnisotropicClamp();


	private:
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> deviceContext;

		// Blend states
		ComPtr<ID3D11BlendState> opaque;
		ComPtr<ID3D11BlendState> alphaBlend;
		ComPtr<ID3D11BlendState> additive;
		ComPtr<ID3D11BlendState> nonPremultiplied;

		// Depth stencil states
		ComPtr<ID3D11DepthStencilState> depthNone;
		ComPtr<ID3D11DepthStencilState> depthDefault;
		ComPtr<ID3D11DepthStencilState> depthRead;

		// Rasterizer states
		ComPtr<ID3D11RasterizerState> cullNone;
		ComPtr<ID3D11RasterizerState> cullClockwise;
		ComPtr<ID3D11RasterizerState> cullCounterClockwise;
		ComPtr<ID3D11RasterizerState> wireframe;

		// Sampler states
		ComPtr<ID3D11SamplerState> pointWrap;
		ComPtr<ID3D11SamplerState> pointClamp;
		ComPtr<ID3D11SamplerState> linearWrap;
		ComPtr<ID3D11SamplerState> linearClamp;
		ComPtr<ID3D11SamplerState> anisotropicWrap;
		ComPtr<ID3D11SamplerState> anisotropicClamp;
};