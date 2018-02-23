#pragma once

#include <filesystem>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <DirectXHelpers.h>

#include "util\engine_util.h"
#include "util\datatypes\datatypes.h"
#include "texture\texture.h"


using std::experimental::filesystem::exists;
using namespace DirectX;


class TextureMgr {
	public:
		static TextureMgr* Get();

		TextureMgr() = default;
		~TextureMgr() = default;

		const shared_ptr<Texture> CreateTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, wstring filename);
		const shared_ptr<Texture> CreateColorTexture(ID3D11Device* device, XMFLOAT4 color);
		const shared_ptr<Texture> CreateTexture2DArray(ID3D11Device* device, ID3D11DeviceContext* device_context, vector<wstring> filenames);


	private:
		shared_ptr<Texture> NewTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, wstring filename);
		shared_ptr<Texture> NewTexture2DArray(ID3D11Device* device, ID3D11DeviceContext* device_context, vector<wstring> filenames);
		shared_ptr<Texture> NewColorTexture(ID3D11Device* device, UINT color);

	private:
		map<wstring, shared_ptr<Texture>>         texture_map;
		map<UINT, shared_ptr<Texture>>            color_texture_map;
		map<vector<wstring>, shared_ptr<Texture>> texture_array_map;
};

