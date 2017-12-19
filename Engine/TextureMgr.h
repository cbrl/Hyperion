#pragma once

#include "EngineUtil.h"
#include <d3d11.h>
#include <WICTextureLoader.h>
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
		TextureMgr(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
		~TextureMgr();

		ComPtr<ID3D11ShaderResourceView> CreateTexture(vector<wstring> filenames);


	private:
		ComPtr<ID3D11ShaderResourceView> CreateSingleTexture(vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView> CreateTexture2DArray(vector<wstring> filenames);

	private:
		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
		map<vector<wstring>, ComPtr<ID3D11ShaderResourceView>> m_TextureMap;
};

