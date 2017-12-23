#pragma once

#include "EngineUtil.h"
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <wrl\client.h>
#include <string>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::wstring;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class TextureMgr {
	public:
		TextureMgr(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext);
		~TextureMgr();

		ComPtr<ID3D11ShaderResourceView>& CreateTexture(vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView>& CreateSimpleTexture(wstring name, XMFLOAT4 color);


	private:
		ComPtr<ID3D11ShaderResourceView> SingleTexture(vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView> Texture2DArray(vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView> SimpleTexture(XMFLOAT4 color);

	private:
		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		map<vector<wstring>, ComPtr<ID3D11ShaderResourceView>> m_TextureMap;
};

