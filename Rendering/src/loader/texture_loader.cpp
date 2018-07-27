#include "texture_loader.h"
#include <DirectXTex.h>
#include "io/io.h"
#include "log/log.h"


namespace TextureLoader {

void LoadTexture(ID3D11Device& device,
                 ID3D11DeviceContext& device_context,
                 const std::wstring& filename,
                 ID3D11ShaderResourceView** srv_out) {

	// Return white texture if the file is missing
	if (!fs::exists(filename)) {
		Logger::log(LogLevel::err, "Error loading texture (file not found): {}", wstr2str(filename));
		return LoadTexture(device, 0xFFFFFFFF, srv_out);
	}

	const std::wstring ext = GetFileExtension(filename);

	// DDS
	if (ext == L".dds") {
		ThrowIfFailed(CreateDDSTextureFromFile(&device, filename.c_str(), nullptr, srv_out),
		              "Failed to create DDS texture");
	}

	// TGA
	else if (ext == L".tga") {
		ScratchImage image;
		LoadFromTGAFile(filename.c_str(), nullptr, image);

		ComPtr<ID3D11Resource> texture;
		ThrowIfFailed(CreateTexture(&device,
		                            image.GetImages(),
		                            image.GetImageCount(),
		                            image.GetMetadata(),
		                            texture.GetAddressOf()),
		              "Failed to create TGA texture");

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format              = image.GetMetadata().format;
		srv_desc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = static_cast<u32>(image.GetMetadata().mipLevels);

		ThrowIfFailed(device.CreateShaderResourceView(texture.Get(), &srv_desc, srv_out),
					  "Failed to create SRV");
	}

	// WIC
	else {
		ThrowIfFailed(CreateWICTextureFromFile(&device, &device_context, filename.c_str(), nullptr, srv_out),
		              "Failed to create WIC texture");
	}

	SetDebugObjectName(*srv_out, "TextureLoader Texture");
	Logger::log(LogLevel::debug, "Loaded texture: {}", wstr2str(filename));
}




void LoadTexture(ID3D11Device& device,
                 u32 color,
                 ID3D11ShaderResourceView** srv_out) {

	// Create a texture from the color
	ComPtr<ID3D11Texture2D> texture;
	D3D11_SUBRESOURCE_DATA init_data = {&color, sizeof(u32), 0};

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width                = desc.Height = desc.MipLevels = desc.ArraySize = 1;
	desc.Format               = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count     = 1;
	desc.Usage                = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags            = D3D11_BIND_SHADER_RESOURCE;

	ThrowIfFailed(device.CreateTexture2D(&desc, &init_data, texture.GetAddressOf()),
	              "Failed to create texture");


	// Create the SRV from the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format                          = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels             = 1;

	ThrowIfFailed(device.CreateShaderResourceView(texture.Get(), &srv_desc, srv_out),
	              "Failed to create SRV");

	SetDebugObjectName(*srv_out, "TextureLoader ColorTexture");
	Logger::log(LogLevel::debug, "Created single color texture (color: {x})", color);
}




void LoadTexture(ID3D11Device& device,
                 ID3D11DeviceContext& device_context,
                 const std::vector<std::wstring>& filenames,
                 ID3D11ShaderResourceView** srv_out) {

	// Return white texture if a file is missing
	for (const std::wstring& fn : filenames) {
		if (!fs::exists(fn)) {
			Logger::log(LogLevel::err, "Error loading texture (file not found): {}", wstr2str(fn));
			return LoadTexture(device, 0xFFFFFFFF, srv_out);
		}
	}

	// Create a vector of textures
	const u32 size = static_cast<u32>(filenames.size());
	std::vector<ComPtr<ID3D11Texture2D>> srcTex(size);

	for (u32 i = 0; i < size; i++) {
		if (GetFileExtension(filenames[i]) == L".dds") {
			ThrowIfFailed(CreateDDSTextureFromFileEx(&device,
			                                         filenames[i].c_str(),
			                                         NULL,
			                                         D3D11_USAGE_STAGING,
			                                         NULL,
			                                         D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
			                                         NULL,
			                                         false,
			                                         reinterpret_cast<ID3D11Resource**>(srcTex[i].GetAddressOf()),
			                                         nullptr),
			              "Failed to create DDS texture");
		}
		else {
			ThrowIfFailed(CreateWICTextureFromFileEx(&device,
			                                         &device_context,
			                                         filenames[i].c_str(),
			                                         NULL,
			                                         D3D11_USAGE_STAGING,
			                                         NULL,
			                                         D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
			                                         NULL,
			                                         NULL,
			                                         reinterpret_cast<ID3D11Resource**>(srcTex[i].GetAddressOf()),
			                                         nullptr),
			              "Failed to create WIC texture");
		}
	}


	// Get the texture description from a texture
	D3D11_TEXTURE2D_DESC desc = {};
	srcTex[0].Get()->GetDesc(&desc);

	// Create the texture array description
	D3D11_TEXTURE2D_DESC array_desc = {};
	array_desc.Width                = desc.Width;
	array_desc.Height               = desc.Height;
	array_desc.MipLevels            = desc.MipLevels;
	array_desc.ArraySize            = size;
	array_desc.Format               = desc.Format;
	array_desc.SampleDesc.Count     = 1;
	array_desc.SampleDesc.Quality   = 0;
	array_desc.Usage                = D3D11_USAGE_DEFAULT;
	array_desc.BindFlags            = D3D11_BIND_SHADER_RESOURCE;
	array_desc.CPUAccessFlags       = 0;
	array_desc.MiscFlags            = 0;

	// Create texture array
	ComPtr<ID3D11Texture2D> tex_array;
	ThrowIfFailed(device.CreateTexture2D(&array_desc, nullptr, tex_array.GetAddressOf()),
	              "Failed to create texture array");


	// Update texture array with texture data
	for (u32 texElement                        = 0; texElement < size; texElement++) {
		for (u32 mipLevel                      = 0; mipLevel < desc.MipLevels; mipLevel++) {
			D3D11_MAPPED_SUBRESOURCE mappedTex = {};

			ThrowIfFailed(device_context.Map(srcTex[texElement].Get(), mipLevel, D3D11_MAP_READ, NULL, &mappedTex),
			              "Failed to std::map texture element");

			device_context.UpdateSubresource(tex_array.Get(),
			                                 D3D11CalcSubresource(mipLevel, texElement, desc.MipLevels),
			                                 nullptr,
			                                 mappedTex.pData,
			                                 mappedTex.RowPitch,
			                                 mappedTex.DepthPitch);

			device_context.Unmap(srcTex[texElement].Get(), mipLevel);
		}
	}


	// Create SRV description
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
	view_desc.Format                          = array_desc.Format;
	view_desc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	view_desc.Texture2DArray.MipLevels        = array_desc.MipLevels;
	view_desc.Texture2DArray.MostDetailedMip  = 0;
	view_desc.Texture2DArray.FirstArraySlice  = 0;
	view_desc.Texture2DArray.ArraySize        = size;

	// Create the SRV
	ThrowIfFailed(device.CreateShaderResourceView(tex_array.Get(), &view_desc, srv_out),
	              "Failed to create SRV");

	SetDebugObjectName(*srv_out, "TextureLoader Texture2DArray");
}
}
