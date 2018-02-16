#include "stdafx.h"
#include "TextureMgr.h"
#include "rendering\RenderingMgr.h"

TextureMgr* TextureMgr::Get() {
	assert(RenderingMgr::Get());

	return RenderingMgr::Get()->GetTextureMgr();
}


TextureMgr::TextureMgr() {
}


TextureMgr::~TextureMgr() {
	texture_map.clear();
	simple_texture_map.clear();
	texture_array_map.clear();
}


// Create a single texture from a file (jpg, png, etc...)
const ComPtr<ID3D11ShaderResourceView>& TextureMgr::Texture(ID3D11Device* device, ID3D11DeviceContext* device_context, wstring filename) {
	// Check for valid file
	if (filename.empty())
		return nullptr;
	if (!exists(filename))
		return nullptr;

	// Create the texture if it doesn't exist
	if (texture_map.find(filename) == texture_map.end()) {
		texture_map[filename] = CreateSingleTexture(device, device_context, filename);
		return texture_map.at(filename);
	}
	else {
		return texture_map.at(filename);
	}
}


// Create a single texture from a float4 (r, g, b, a)
const ComPtr<ID3D11ShaderResourceView>& TextureMgr::PlainTexture(ID3D11Device* device, XMFLOAT4 color) {
	// Convert the float4 into a single hex color value, which is also used as the texture data when creating it
	UINT texColor = (UINT)(color.x * 0xff) + ((UINT)(color.y * 0xff) << 8) + ((UINT)(color.z * 0xff) << 16) + ((UINT)(color.w * 0xff) << 24);

	if (simple_texture_map.find(texColor) == simple_texture_map.end()) {
		simple_texture_map[texColor] = CreatePlainTexture(device, texColor);
		return simple_texture_map.at(texColor);
	}
	else {
		return simple_texture_map.at(texColor);
	}
}

// Create a Texture2DArray from multiple files
const ComPtr<ID3D11ShaderResourceView>& TextureMgr::Texture2DArray(ID3D11Device* device, ID3D11DeviceContext* device_context, vector<wstring> filenames) {
	// Check for valid file
	if (filenames.empty())
		return nullptr;
	for (auto& file : filenames) {
		if (!exists(file))
			return nullptr;
	}

	// Create the texture if it doesn't exist
	if (texture_array_map.find(filenames) == texture_array_map.end()) {
		texture_array_map[filenames] = CreateTexture2DArray(device, device_context, filenames);
		return texture_array_map.at(filenames);
	}
	else {
		return texture_array_map.at(filenames);
	}
}


ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateSingleTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, wstring filename) {
	ComPtr<ID3D11ShaderResourceView> textureSRV;

	// Create a shader resource view
	DX::ThrowIfFailed(CreateWICTextureFromFile(device, device_context, filename.c_str(), nullptr, textureSRV.GetAddressOf()),
	                  "Failed to create WIC texture");

	SetDebugObjectName(textureSRV.Get(), "TextureMgr Texture");

	return textureSRV;
}


ComPtr<ID3D11ShaderResourceView> TextureMgr::CreateTexture2DArray(ID3D11Device* device, ID3D11DeviceContext* device_context, vector<wstring> filenames) {
	ComPtr<ID3D11ShaderResourceView> textureSRV;

	size_t size = filenames.size();
	vector<ComPtr<ID3D11Texture2D>> srcTex(size);

	// Create a vector of textures
	for(size_t i = 0; i < size; i++) {
		DX::ThrowIfFailed(CreateWICTextureFromFileEx(device, device_context, filenames[i].c_str(),
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

			DX::ThrowIfFailed(device_context->Map(srcTex[texElement].Get(), mipLevel, D3D11_MAP_READ, NULL, &mappedTex),
			                  "Failed to map texture element");

			device_context->UpdateSubresource(texArray.Get(), D3D11CalcSubresource(mipLevel, texElement, desc.MipLevels), nullptr,
											   mappedTex.pData, mappedTex.RowPitch, mappedTex.DepthPitch);

			device_context->Unmap(srcTex[texElement].Get(), mipLevel);
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

	SetDebugObjectName(texArray.Get(), "TextureMgr Texture2DArray");


	return textureSRV;
}


ComPtr<ID3D11ShaderResourceView> TextureMgr::CreatePlainTexture(ID3D11Device* device, UINT color) {
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

	SetDebugObjectName(textureSRV.Get(), "TextureMgr PlainTexture");

	return textureSRV;
}