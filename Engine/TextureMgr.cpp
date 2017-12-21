#include "stdafx.h"
#include "TextureMgr.h"


TextureMgr::TextureMgr(const ComPtr<ID3D11Device>& device, const ComPtr<ID3D11DeviceContext>& deviceContext) :
	m_Device(device),
	m_DeviceContext(deviceContext)
{
}


TextureMgr::~TextureMgr() {
	m_TextureMap.clear();
}

ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateTexture(vector<wstring> filenames) {
	// Return the texture if it was already created, or create it if not
	if (m_TextureMap.find(filenames) != m_TextureMap.end()) {
		return m_TextureMap[filenames];
	}
	else {
		if (filenames.size() == 1) {
			m_TextureMap[filenames] = CreateSingleTexture(filenames);
			return m_TextureMap[filenames];
		}
		else {
			m_TextureMap[filenames] = CreateTexture2DArray(filenames);
			return m_TextureMap[filenames];
		}
	}
}

ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateSingleTexture(vector<wstring> filenames) {
	ComPtr<ID3D11ShaderResourceView> textureView = nullptr;

	CreateWICTextureFromFile(m_Device.Get(), m_DeviceContext.Get(), filenames[0].c_str(), nullptr, textureView.GetAddressOf());

	return textureView;
}

ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateTexture2DArray(vector<wstring> filenames) {
	ComPtr<ID3D11ShaderResourceView> textureView = nullptr;

	size_t size = filenames.size();
	vector<ComPtr<ID3D11Texture2D>> srcTex(size);

	for(size_t i = 0; i < size; i++) {
		HR(CreateWICTextureFromFile(m_Device.Get(), m_DeviceContext.Get(), filenames[i].c_str(),
		                            (ID3D11Resource**)srcTex[i].GetAddressOf(), nullptr));
	}

	D3D11_TEXTURE2D_DESC desc;
	srcTex[0].Get()->GetDesc(&desc);

	D3D11_TEXTURE2D_DESC arrayDesc;
	arrayDesc.Width = desc.Width;
	arrayDesc.Height = desc.Height;
	arrayDesc.MipLevels = desc.MipLevels;
	arrayDesc.ArraySize = size;
	arrayDesc.Format = desc.Format;
	arrayDesc.SampleDesc.Count = 1;
	arrayDesc.SampleDesc.Quality = 0;
	arrayDesc.Usage = D3D11_USAGE_STAGING;
	arrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	arrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	arrayDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texArray = nullptr;
	HR(m_Device->CreateTexture2D(&arrayDesc, nullptr, texArray.GetAddressOf()));

	for (UINT texElement = 0; texElement < size; texElement++) {
		for (UINT mipLevel = 0; mipLevel < desc.MipLevels; mipLevel++) {
			D3D11_MAPPED_SUBRESOURCE mappedTex;

			HR(m_DeviceContext->Map(srcTex[texElement].Get(), mipLevel, D3D11_MAP_READ, NULL, &mappedTex));

			m_DeviceContext->UpdateSubresource(texArray.Get(), D3D11CalcSubresource(mipLevel, texElement, desc.MipLevels), nullptr,
											   mappedTex.pData, mappedTex.RowPitch, mappedTex.DepthPitch);

			m_DeviceContext->Unmap(srcTex[texElement].Get(), mipLevel);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = arrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = arrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	HR(m_Device->CreateShaderResourceView(texArray.Get(), &viewDesc, textureView.GetAddressOf()));


	return textureView;
}


