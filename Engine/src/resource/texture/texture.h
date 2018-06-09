#pragma once

#include <d3d11.h>
#include "datatypes/datatypes.h"
#include "resource/resource.h"
#include "rendering/pipeline.h"
#include "loader/texture_loader.h"


struct Texture final : public Resource<Texture> {
public:
	Texture() = default;
	~Texture() = default;

	Texture(ID3D11Device& device,
	        ID3D11DeviceContext& device_context,
	        wstring filename) {

		TextureLoader::LoadTexture(device, device_context, filename, texture.GetAddressOf());
	}

	Texture(ID3D11Device& device, u32 color) {

		TextureLoader::LoadTexture(device, color, texture.GetAddressOf());
	}


	ID3D11ShaderResourceView* Get() const {
		return texture.Get();
	}

	// Bind the texture to the specified pipeline stage
	template<typename StageT>
	void Bind(ID3D11DeviceContext& device_context, u32 slot) const {

		StageT::BindSRVs(device_context, slot, 1, texture.GetAddressOf());
	}


private:
	ComPtr<ID3D11ShaderResourceView> texture;
};
