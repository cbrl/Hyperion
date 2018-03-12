#include "stdafx.h"
#include "texture_factory.h"


// Create a single texture from a file (jpg, png, etc...)
const shared_ptr<Texture> TextureFactory::CreateTexture(ID3D11Device* device, ID3D11DeviceContext* device_context, wstring filename) {
	
	ComPtr<ID3D11ShaderResourceView> texture_srv;

	// Create a shader resource view
	ThrowIfFailed(CreateWICTextureFromFile(device, device_context, filename.c_str(), nullptr, texture_srv.GetAddressOf()),
				  "Failed to create WIC texture");

	SetDebugObjectName(texture_srv.Get(), "TextureFactory Texture");

	return make_shared<Texture>(texture_srv);
}


// Create a single texture from a float4 (r, g, b, a)
const shared_ptr<Texture> TextureFactory::CreateColorTexture(ID3D11Device* device, u32 color) {
	
	

	ComPtr<ID3D11Texture2D>          texture;
	ComPtr<ID3D11ShaderResourceView> texture_srv;

	D3D11_SUBRESOURCE_DATA initData = { &color, sizeof(u32), 0 };

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ThrowIfFailed(device->CreateTexture2D(&desc, &initData, texture.GetAddressOf()),
				  "Failed to create texture");

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;

	ThrowIfFailed(device->CreateShaderResourceView(texture.Get(), &srv_desc, texture_srv.GetAddressOf()),
				  "Failed to create SRV");

	SetDebugObjectName(texture_srv.Get(), "TextureFactory ColorTexture");

	return make_shared<Texture>(texture_srv);
}


// Create a Texture2DArray from multiple files
const shared_ptr<Texture> TextureFactory::CreateTexture2DArray(ID3D11Device* device, ID3D11DeviceContext* device_context, vector<wstring> filenames) {
	
	ComPtr<ID3D11ShaderResourceView> texture_srv;

	u32 size = static_cast<u32>(filenames.size());
	vector<ComPtr<ID3D11Texture2D>> srcTex(size);

	// Create a vector of textures
	for (u32 i = 0; i < size; i++) {
		ThrowIfFailed(CreateWICTextureFromFileEx(device, device_context, filenames[i].c_str(),
												 NULL, D3D11_USAGE_STAGING, NULL, D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE, NULL, NULL,
												 (ID3D11Resource**)srcTex[i].GetAddressOf(), nullptr),
					  "Failed to create WIC texture");
	}

	// Get the texture description from a texture
	D3D11_TEXTURE2D_DESC desc = {};
	srcTex[0].Get()->GetDesc(&desc);

	// Create the texture array description
	D3D11_TEXTURE2D_DESC array_desc = {};
	array_desc.Width = desc.Width;
	array_desc.Height = desc.Height;
	array_desc.MipLevels = desc.MipLevels;
	array_desc.ArraySize = size;
	array_desc.Format = desc.Format;
	array_desc.SampleDesc.Count = 1;
	array_desc.SampleDesc.Quality = 0;
	array_desc.Usage = D3D11_USAGE_DEFAULT;
	array_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	array_desc.CPUAccessFlags = 0;
	array_desc.MiscFlags = 0;

	// Create texture array
	ComPtr<ID3D11Texture2D> tex_array;
	ThrowIfFailed(device->CreateTexture2D(&array_desc, NULL, tex_array.GetAddressOf()),
				  "Failed to create texture array");

	// Update texture array with texture data
	for (u32 texElement = 0; texElement < size; texElement++) {
		for (u32 mipLevel = 0; mipLevel < desc.MipLevels; mipLevel++) {
			D3D11_MAPPED_SUBRESOURCE mappedTex = {};

			ThrowIfFailed(device_context->Map(srcTex[texElement].Get(), mipLevel, D3D11_MAP_READ, NULL, &mappedTex),
						  "Failed to map texture element");

			device_context->UpdateSubresource(tex_array.Get(), D3D11CalcSubresource(mipLevel, texElement, desc.MipLevels), nullptr,
											  mappedTex.pData, mappedTex.RowPitch, mappedTex.DepthPitch);

			device_context->Unmap(srcTex[texElement].Get(), mipLevel);
		}
	}

	// Create SRV description
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = array_desc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MipLevels = array_desc.MipLevels;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	// Create the SRV
	ThrowIfFailed(device->CreateShaderResourceView(tex_array.Get(), &viewDesc, texture_srv.GetAddressOf()),
				  "Failed to create SRV");

	SetDebugObjectName(tex_array.Get(), "TextureFactory Texture2DArray");


	return make_shared<Texture>(texture_srv);
}