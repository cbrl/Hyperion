#pragma once

#include <d3d11.h>
#include "util\datatypes\datatypes.h"
#include "rendering\pipeline.h"
#include "loader\texture_loader.h"


struct Texture final {
	public:
		Texture() = default;
		~Texture() = default;

		template<typename... ArgsT>
		Texture(ArgsT&&... args) {

			// Create the texture SRV
			TextureLoader::LoadTexture(std::forward<ArgsT>(args)..., texture.GetAddressOf());
		}

		Texture(ID3D11ShaderResourceView* texture) : texture(texture) {}

		Texture(ComPtr<ID3D11ShaderResourceView> texture) : texture(texture) {}

		// Bind the texture to the specified pipeline stage
		template<typename StageT>
		void Bind(ID3D11DeviceContext* device_context, u32 slot) {

			StageT::BindSRVs(device_context, slot, 1, texture.GetAddressOf());
		}


	private:
		ComPtr<ID3D11ShaderResourceView> texture;
};