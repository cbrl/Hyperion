#pragma once

#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <DirectXHelpers.h>

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "resource\texture\texture.h"


class TextureFactory {
	public:
		TextureFactory() = default;
		~TextureFactory() = default;

		const shared_ptr<Texture> CreateTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, wstring filename);
		const shared_ptr<Texture> CreateColorTexture(ID3D11Device* device, u32 color);
		const shared_ptr<Texture> CreateTexture2DArray(ID3D11Device* device, ID3D11DeviceContext* device_context, vector<wstring> filenames);


	private:
		map<wstring, shared_ptr<Texture>>         texture_map;
		map<u32, shared_ptr<Texture>>             color_texture_map;
		map<vector<wstring>, shared_ptr<Texture>> texture_array_map;
};

