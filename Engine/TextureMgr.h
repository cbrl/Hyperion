#pragma once

#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <wrl\client.h>
#include <string>
#include <vector>
#include <map>
#include "EngineUtil.h"

using std::map;
using std::vector;
using std::wstring;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

class TextureMgr {
	public:
		TextureMgr(const ComPtr<ID3D11Device> device, const ComPtr<ID3D11DeviceContext> deviceContext);
		~TextureMgr();

		const ComPtr<ID3D11ShaderResourceView>& Texture(vector<wstring> filenames);
		const ComPtr<ID3D11ShaderResourceView>& SimpleTexture(XMFLOAT4 color);


	private:
		ComPtr<ID3D11ShaderResourceView> CreateSingleTexture(vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView> CreateTexture2DArray(vector<wstring> filenames);
		ComPtr<ID3D11ShaderResourceView> CreateSimpleTexture(UINT color);

	private:
		map<vector<wstring>, ComPtr<ID3D11ShaderResourceView>> m_TextureMap;
		map<UINT, ComPtr<ID3D11ShaderResourceView>> m_SimpleTextureMap;

		ComPtr<ID3D11Device>        m_Device;
		ComPtr<ID3D11DeviceContext> m_DeviceContext;
};
