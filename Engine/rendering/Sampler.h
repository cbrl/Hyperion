#pragma once

#include <d3d11.h>
#include <wrl\client.h>
#include "util\EngineUtil.h"

using Microsoft::WRL::ComPtr;

class Sampler {
	public:
		Sampler(const ComPtr<ID3D11Device>& device) {
			CreateSampler(device);
		}
		~Sampler() = default;

	private:
		void CreateSampler(const ComPtr<ID3D11Device>& device) {
			D3D11_SAMPLER_DESC samplerDesc;

			// Create texture sampler description
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			HR(device->CreateSamplerState(&samplerDesc, samplerState.ReleaseAndGetAddressOf()));
		}

	public:
		ComPtr<ID3D11SamplerState> samplerState;
};