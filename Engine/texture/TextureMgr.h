#pragma once

#include <string>
#include <vector>
#include <map>
#include <wrl\client.h>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <DirectXHelpers.h>

#include "util\EngineUtil.h"

using std::map;
using std::vector;
using std::wstring;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class TextureMgr {
	public:
		TextureMgr(const ComPtr<ID3D11Device> device, const ComPtr<ID3D11DeviceContext> deviceContext);
		~TextureMgr();

		const ComPtr<ID3D11ShaderResourceView>& Texture(wstring filename);
		const ComPtr<ID3D11ShaderResourceView>& SimpleTexture(XMFLOAT4 color);
		const ComPtr<ID3D11ShaderResourceView>& Texture2DArray(vector<wstring> filenames);


	private:
		ComPtr<ID3D11ShaderResourceView> CreateSingleTexture(wstring filename);
		ComPtr<ID3D11ShaderResourceView> CreateTexture2DArray(vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView> CreateSimpleTexture(UINT color);

	private:
		ComPtr<ID3D11Device>        device;
		ComPtr<ID3D11DeviceContext> deviceContext;

		map<wstring, ComPtr<ID3D11ShaderResourceView>>         textureMap;
		map<UINT, ComPtr<ID3D11ShaderResourceView>>            simpleTextureMap;
		map<vector<wstring>, ComPtr<ID3D11ShaderResourceView>> multiTextureMap;
};

