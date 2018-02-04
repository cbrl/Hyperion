#include "stdafx.h"
#include "TextureMgr.h"


TextureMgr::TextureMgr(const ComPtr<ID3D11Device> device, const ComPtr<ID3D11DeviceContext> deviceContext) :
	device(device),
	deviceContext(deviceContext)
{
}


TextureMgr::~TextureMgr() {
	m_TextureMap.clear();
	m_SimpleTextureMap.clear();
}


const ComPtr<ID3D11ShaderResourceView>& TextureMgr::Texture(vector<wstring> filenames) {
	// Create the texture if it doesn't exist
	if (m_TextureMap.find(filenames) == m_TextureMap.end()) {
		if (filenames.size() == 1) {
			m_TextureMap[filenames] = CreateSingleTexture(filenames);
			return m_TextureMap.at(filenames);
		}
		else {
			m_TextureMap[filenames] = CreateTexture2DArray(filenames);
			return m_TextureMap.at(filenames);
		}
	}
	else {
		return m_TextureMap.at(filenames);
	}
}


const ComPtr<ID3D11ShaderResourceView>& TextureMgr::SimpleTexture(XMFLOAT4 color) {
	// Convert the float4 into a single hex color value, which is also used as the texture data when creating it
	UINT texColor = (UINT)(color.x * 0xff) + ((UINT)(color.y * 0xff) << 8) + ((UINT)(color.z * 0xff) << 16) + ((UINT)(color.w * 0xff) << 24);

	if (m_SimpleTextureMap.find(texColor) == m_SimpleTextureMap.end()) {
		m_SimpleTextureMap[texColor] = CreateSimpleTexture(texColor);
		return m_SimpleTextureMap.at(texColor);
	}
	else {
		return m_SimpleTextureMap.at(texColor);
	}
}


ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateSingleTexture(vector<wstring> filenames) {
	ComPtr<ID3D11ShaderResourceView> textureView;

	// Create a shader resource view
	DX::ThrowIfFailed(CreateWICTextureFromFile(device.Get(), deviceContext.Get(), filenames[0].c_str(), nullptr, textureView.GetAddressOf()),
	                  "Failed to create WIC texture");

	return textureView;
}


ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateTexture2DArray(vector<wstring> filenames) {
	ComPtr<ID3D11ShaderResourceView> textureSRV;

	size_t size = filenames.size();
	vector<ComPtr<ID3D11Texture2D>> srcTex(size);

	// Create a vector of textures
	for(size_t i = 0; i < size; i++) {
		DX::ThrowIfFailed(CreateWICTextureFromFileEx(device.Get(), deviceContext.Get(), filenames[i].c_str(),
									                 NULL, D3D11_USAGE_STAGING, NULL,D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE, NULL, NULL,
									                 (ID3D11Resource**)srcTex[i].GetAddressOf(), nullptr),
		                  "Failed to create WIC texture");
	}

	// Get the texture description from a texture
	D3D11_TEXTURE2D_DESC desc = {};
	srcTex[0].Get()->GetDesc(&desc);

	// Create the texture array description
	D3D11_TEXTURE2D_DESC arrayDesc = {};
	arrayDesc.Width              = desc.Width;
	arrayDesc.Height             = desc.Height;
	arrayDesc.MipLevels          = desc.MipLevels;
	arrayDesc.ArraySize          = size;
	arrayDesc.Format             = desc.Format;
	arrayDesc.SampleDesc.Count   = 1;
	arrayDesc.SampleDesc.Quality = 0;
	arrayDesc.Usage              = D3D11_USAGE_DEFAULT;
	arrayDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	arrayDesc.CPUAccessFlags     = 0;
	arrayDesc.MiscFlags          = 0;

	// Create texture array
	ComPtr<ID3D11Texture2D> texArray;
	DX::ThrowIfFailed(device->CreateTexture2D(&arrayDesc, NULL, texArray.GetAddressOf()),
	                  "Failed to create texture array");

	// Update texture array with texture data
	for (UINT texElement = 0; texElement < size; texElement++) {
		for (UINT mipLevel = 0; mipLevel < desc.MipLevels; mipLevel++) {
			D3D11_MAPPED_SUBRESOURCE mappedTex = {};

			DX::ThrowIfFailed(deviceContext->Map(srcTex[texElement].Get(), mipLevel, D3D11_MAP_READ, NULL, &mappedTex),
			                  "Failed to map texture element");

			deviceContext->UpdateSubresource(texArray.Get(), D3D11CalcSubresource(mipLevel, texElement, desc.MipLevels), nullptr,
											   mappedTex.pData, mappedTex.RowPitch, mappedTex.DepthPitch);

			deviceContext->Unmap(srcTex[texElement].Get(), mipLevel);
		}
	}

	// Create SRV description
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format                         = arrayDesc.Format;
	viewDesc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MipLevels       = arrayDesc.MipLevels;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize       = size;

	// Create the SRV
	DX::ThrowIfFailed(device->CreateShaderResourceView(texArray.Get(), &viewDesc, textureSRV.GetAddressOf()),
	                  "Failed to create SRV");


	return textureSRV;
}


ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateSimpleTexture(UINT color) {
	ComPtr<ID3D11Texture2D>          texture;
	ComPtr<ID3D11ShaderResourceView> textureSRV;

	//UINT texColor = (UINT)(color.x * 0xff) + ((UINT)(color.y * 0xff) << 8) + ((UINT)(color.z * 0xff) << 16) + ((UINT)(color.w * 0xff) << 24);

	D3D11_SUBRESOURCE_DATA initData = { &color, sizeof(UINT), 0 };

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
	desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage            = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

	DX::ThrowIfFailed(device->CreateTexture2D(&desc, &initData, texture.GetAddressOf()),
	                  "Failed to create texture");

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format              = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	DX::ThrowIfFailed(device->CreateShaderResourceView(texture.Get(), &SRVDesc, textureSRV.GetAddressOf()),
	                  "Failed to create SRV");

	return textureSRV;
}