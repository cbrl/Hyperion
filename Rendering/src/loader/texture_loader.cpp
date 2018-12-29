#include "texture_loader.h"
#include <DirectXTex.h>
#include "io/io.h"
#include "log/log.h"


u32 error_tex_data[128][128] = {{}};

// Create data for a checkerboard pattern texture
void CreateErrorTextureData() {

	constexpr u32 color  = 0xFF0000FF;
	constexpr u32 color2 = 0xFF000000;

	constexpr auto x_size = std::size(error_tex_data[0]);
	constexpr auto y_size = std::size(error_tex_data);

	constexpr auto x_half_size = x_size / 2;
	constexpr auto y_half_size = y_size / 2;

	for (size_t i = 0; i < y_half_size; ++i) {

		for (size_t j = 0; j < x_half_size; ++j) {
			error_tex_data[i][j] = color;
		}
		for (size_t j = x_half_size; j < x_size; ++j) {
			error_tex_data[i][j] = color2;
		}
	}

	for (size_t i = y_half_size; i < y_size; ++i) {

		for (size_t j = 0; j < x_half_size; ++j) {
			error_tex_data[i][j] = color2;
		}
		for (size_t j = x_half_size; j < x_size; ++j) {
			error_tex_data[i][j] = color;
		}
	}
}


void CreateErrorTexture(ID3D11Device& device, ID3D11ShaderResourceView** srv_out) {

	static bool error_tex_initialized = false;
	if (!error_tex_initialized) {
		CreateErrorTextureData();
		error_tex_initialized = true;
	}

	ComPtr<ID3D11Texture2D> texture;

	{
		D3D11_SUBRESOURCE_DATA init_data = { error_tex_data, sizeof(u32) * 128, 0 };

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width            = 128;
		desc.Height           = 128;
		desc.MipLevels        = 1;
		desc.ArraySize        = 1;
		desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage            = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

		ThrowIfFailed(device.CreateTexture2D(&desc, &init_data, texture.GetAddressOf()),
		              "Failed to create Texture2D");
	}

	ThrowIfFailed(device.CreateShaderResourceView(texture.Get(), nullptr, srv_out),
	              "Failed to create SRV");

	SetDebugObjectName(*srv_out, "TextureLoader error texture");
	Logger::log(LogLevel::debug, "Created error texture");
}


void HandleLoaderError(ID3D11Device& device, const std::string& msg, ID3D11ShaderResourceView** srv_out) {
	Logger::log(LogLevel::err, msg);
	CreateErrorTexture(device, srv_out);
}




namespace TextureLoader {

void load(ID3D11Device& device,
          ID3D11DeviceContext& device_context,
          const std::wstring& filename,
          ID3D11ShaderResourceView** srv_out) {

	// Return white texture if the file is missing
	if (!fs::exists(filename)) {
		HandleLoaderError(device, "Error loading texture (file not found): " + WstrToStr(filename), srv_out);
		return;
	}

	const std::wstring ext = GetFileExtension(filename);

	// DDS
	if (ext == L".dds") {
		HRESULT hr = CreateDDSTextureFromFile(&device, filename.c_str(), nullptr, srv_out);
		if (FAILED(hr)) {
			HandleLoaderError(device, "Failed to create DDS texture: " + WstrToStr(filename), srv_out);
			return;
		}
	}

	// TGA
	else if (ext == L".tga") {
		HRESULT hr;

		ScratchImage image;
		hr = LoadFromTGAFile(filename.c_str(), nullptr, image);
		if (FAILED(hr)) {
			HandleLoaderError(device, "Failed to load TGA texture: " + WstrToStr(filename), srv_out);
			return;
		}

		ComPtr<ID3D11Resource> texture;
		hr = CreateTexture(&device,
		                   image.GetImages(),
		                   image.GetImageCount(),
		                   image.GetMetadata(),
		                   texture.GetAddressOf());
		if (FAILED(hr)) {
			HandleLoaderError(device, "Failed to create ID3D11Resource: " + WstrToStr(filename), srv_out);
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format              = image.GetMetadata().format;
		srv_desc.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = static_cast<u32>(image.GetMetadata().mipLevels);

		hr = device.CreateShaderResourceView(texture.Get(), &srv_desc, srv_out);
		if (FAILED(hr)) {
			HandleLoaderError(device, "Failed to create SRV: " + WstrToStr(filename), srv_out);
			return;
		}
	}

	// WIC
	else {
		HRESULT hr = CreateWICTextureFromFile(&device, &device_context, filename.c_str(), nullptr, srv_out);
		if (FAILED(hr)) {
			HandleLoaderError(device, "Failed to create WIC texture: " + WstrToStr(filename), srv_out);
			return;
		}
	}

	SetDebugObjectName(*srv_out, "TextureLoader Texture");
	Logger::log(LogLevel::debug, "Loaded texture: {}", WstrToStr(filename));
}


void load(ID3D11Device& device,
          ID3D11DeviceContext& device_context,
          const std::vector<std::wstring>& filenames,
          ID3D11ShaderResourceView** srv_out) {

	// Return white texture if a file is missing
	for (const std::wstring& fn : filenames) {
		if (!fs::exists(fn)) {
			HandleLoaderError(device, "Error loading texture (file not found): " + WstrToStr(fn), srv_out);
			return;
		}
	}

	// Create a vector of textures
	std::vector<ComPtr<ID3D11Texture2D>> src_tex(filenames.size());

	for (size_t i = 0; i < filenames.size(); i++) {
		if (GetFileExtension(filenames[i]) == L".dds") {
			HRESULT hr = CreateDDSTextureFromFileEx(&device,
			                                        filenames[i].c_str(),
			                                        NULL,
			                                        D3D11_USAGE_STAGING,
			                                        NULL,
			                                        D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
			                                        NULL,
			                                        false,
			                                        reinterpret_cast<ID3D11Resource**>(src_tex[i].GetAddressOf()),
			                                        nullptr);
			if (FAILED(hr)) {
				HandleLoaderError(device, "Failed to create Texture2D: " + WstrToStr(filenames[i]), srv_out);
				return;
			}
		}
		else {
			HRESULT hr = CreateWICTextureFromFileEx(&device,
			                                        &device_context,
			                                        filenames[i].c_str(),
			                                        NULL,
			                                        D3D11_USAGE_STAGING,
			                                        NULL,
			                                        D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
			                                        NULL,
			                                        NULL,
			                                        reinterpret_cast<ID3D11Resource**>(src_tex[i].GetAddressOf()),
			                                        nullptr);
			if (FAILED(hr)) {
				HandleLoaderError(device, "Failed to create Texture2D: " + WstrToStr(filenames[i]), srv_out);
				return;
			}
		}
	}


	// Get the texture description from a texture
	D3D11_TEXTURE2D_DESC desc = {};
	src_tex[0].Get()->GetDesc(&desc);

	// Create the texture array description
	D3D11_TEXTURE2D_DESC array_desc = {};
	array_desc.Width              = desc.Width;
	array_desc.Height             = desc.Height;
	array_desc.MipLevels          = desc.MipLevels;
	array_desc.ArraySize          = static_cast<UINT>(filenames.size());
	array_desc.Format             = desc.Format;
	array_desc.SampleDesc.Count   = 1;
	array_desc.SampleDesc.Quality = 0;
	array_desc.Usage              = D3D11_USAGE_DEFAULT;
	array_desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	array_desc.CPUAccessFlags     = 0;
	array_desc.MiscFlags          = 0;

	// Create texture array
	ComPtr<ID3D11Texture2D> tex_array;
	HRESULT hr_tex2d = device.CreateTexture2D(&array_desc, nullptr, tex_array.GetAddressOf());
	if (FAILED(hr_tex2d)) {
		HandleLoaderError(device, "Failed to create Texture2DArray", srv_out);
		return;
	}


	// Update texture array with texture data
	for (size_t texElement = 0; texElement < filenames.size(); texElement++) {
		for (u32 mipLevel = 0; mipLevel < desc.MipLevels; mipLevel++) {
			D3D11_MAPPED_SUBRESOURCE mappedTex = {};

			HRESULT hr = device_context.Map(src_tex[texElement].Get(), mipLevel, D3D11_MAP_READ, NULL, &mappedTex);
			if (FAILED(hr)) {
				HandleLoaderError(device, "Failed to map Texture2D for Texture2DArray", srv_out);
				return;
			}

			device_context.UpdateSubresource(tex_array.Get(),
			                                 D3D11CalcSubresource(mipLevel, static_cast<UINT>(texElement), desc.MipLevels),
			                                 nullptr,
			                                 mappedTex.pData,
			                                 mappedTex.RowPitch,
			                                 mappedTex.DepthPitch);

			device_context.Unmap(src_tex[texElement].Get(), mipLevel);
		}
	}


	// Create SRV description
	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
	view_desc.Format                         = array_desc.Format;
	view_desc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	view_desc.Texture2DArray.MipLevels       = array_desc.MipLevels;
	view_desc.Texture2DArray.MostDetailedMip = 0;
	view_desc.Texture2DArray.FirstArraySlice = 0;
	view_desc.Texture2DArray.ArraySize       = static_cast<UINT>(filenames.size());

	// Create the SRV
	HRESULT hr_srv = device.CreateShaderResourceView(tex_array.Get(), &view_desc, srv_out);
	if (FAILED(hr_srv)) {
		HandleLoaderError(device, "Failed to create Texture2DArray SRV", srv_out);
		return;
	}

	SetDebugObjectName(*srv_out, "TextureLoader Texture2DArray");
}
}
