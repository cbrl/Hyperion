#pragma once

#include <wrl\client.h>
#include <d3d11.h>
#include <DirectXHelpers.h>

#include "util\EngineUtil.h"
#include "shader\HlslDefines.h"
#include "direct3d\Direct3D.h"

using Microsoft::WRL::ComPtr;

using namespace DirectX;

class RenderStateMgr {
	public:
		static const RenderStateMgr* Get();

		RenderStateMgr();
		~RenderStateMgr();

		void SetupStates(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		//----------------------------------------------------------------------------------
		// Bind states
		//----------------------------------------------------------------------------------
		// Blend states
		void BindOpaque(ID3D11DeviceContext* deviceContext, float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;
		void BindAlphaBlend(ID3D11DeviceContext* deviceContext, float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;
		void BindAdditive(ID3D11DeviceContext* deviceContext, float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;
		void BindNonPremultiplied(ID3D11DeviceContext* deviceContext, float blendFactor[4] = {}, unsigned int sampleMask = 0xffffffff) const;

		// Depth stencil states
		void BindDepthNone(ID3D11DeviceContext* deviceContext, unsigned int stencilRef = 0) const;
		void BindDepthDefault(ID3D11DeviceContext* deviceContext, unsigned int stencilRef = 0) const;
		void BindDepthRead(ID3D11DeviceContext* deviceContext, unsigned int stencilRef = 0) const;

		// Rasterizer states
		void BindCullNone(ID3D11DeviceContext* deviceContext) const;
		void BindCullClockwise(ID3D11DeviceContext* deviceContext) const;
		void BindCullCounterClockwise(ID3D11DeviceContext* deviceContext) const;
		void BindWireframe(ID3D11DeviceContext* deviceContext) const;

		// Sampler states
		void BindPointWrap(ID3D11DeviceContext* deviceContext) const;
		void BindPointClamp(ID3D11DeviceContext* deviceContext) const;
		void BindLinearWrap(ID3D11DeviceContext* deviceContext) const;
		void BindLinearClamp(ID3D11DeviceContext* deviceContext) const;
		void BindAnisotropicWrap(ID3D11DeviceContext* deviceContext) const;
		void BindAnisotropicClamp(ID3D11DeviceContext* deviceContext) const;


	private:
		//----------------------------------------------------------------------------------
		// Create states
		//----------------------------------------------------------------------------------
		HRESULT CreateBlendState(ID3D11Device* device, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, _Out_ ID3D11BlendState** pResult);
		HRESULT CreateDepthStencilState(ID3D11Device* device, bool enable, bool writeEnable, _Out_ ID3D11DepthStencilState** pResult);
		HRESULT CreateRasterizerState(ID3D11Device* device, D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, _Out_ ID3D11RasterizerState** pResult);
		HRESULT CreateSamplerState(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, _Out_ ID3D11SamplerState** pResult);

		void CreateBlendStates(ID3D11Device* device);
		void CreateDepthStencilStates(ID3D11Device* device);
		void CreateRasterizerStates(ID3D11Device* device);
		void CreateSamplerStates(ID3D11Device* device);


	private:
		//ComPtr<ID3D11Device>        device;
		//ComPtr<ID3D11DeviceContext> deviceContext;

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