#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"


struct Texture {
	public:
		Texture(ID3D11ShaderResourceView* texture) : texture(texture) {}

		Texture(ComPtr<ID3D11ShaderResourceView> texture) : texture(texture) {}

		~Texture() = default;

		void Bind(ID3D11DeviceContext* device_context, u32 slot) {
			device_context->PSSetShaderResources(slot, 1, texture.GetAddressOf());
		}

	private:
		ComPtr<ID3D11ShaderResourceView> texture;
};