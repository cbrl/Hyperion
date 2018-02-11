#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <wrl\client.h>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <DirectXHelpers.h>

#include "util\EngineUtil.h"

using std::map;
using std::vector;
using std::wstring;
using std::experimental::filesystem::exists;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class TextureMgr {
	public:
		static TextureMgr* Get();

		TextureMgr();
		~TextureMgr();

		const ComPtr<ID3D11ShaderResourceView>& Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring filename);
		const ComPtr<ID3D11ShaderResourceView>& PlainTexture(ID3D11Device* device, XMFLOAT4 color);
		const ComPtr<ID3D11ShaderResourceView>& Texture2DArray(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<wstring> filenames);


	private:
		ComPtr<ID3D11ShaderResourceView> CreateSingleTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, wstring filename);
		ComPtr<ID3D11ShaderResourceView> CreateTexture2DArray(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView> CreatePlainTexture(ID3D11Device* device, UINT color);

	private:
		map<wstring, ComPtr<ID3D11ShaderResourceView>>         textureMap;
		map<UINT, ComPtr<ID3D11ShaderResourceView>>            simpleTextureMap;
		map<vector<wstring>, ComPtr<ID3D11ShaderResourceView>> multiTextureMap;
};

