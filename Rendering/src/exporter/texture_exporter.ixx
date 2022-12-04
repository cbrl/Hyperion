module;

#include "io/io.h"
#include "directx/directxtk.h"
#include <DirectXTex.h>

export module rendering:exporter.texture_exporter;

import :texture;


void GetTexture2D(ID3D11Device& device, const render::Texture& texture, ID3D11Texture2D** data) {
	ComPtr<ID3D11Resource> resource;

	texture.get()->GetResource(resource.GetAddressOf());
	resource->QueryInterface<ID3D11Texture2D>(data);
}

HRESULT ConvertToScratchImage(ID3D11Device& device,
                              ID3D11DeviceContext& device_context,
                              const render::Texture& texture,
                              ScratchImage& output) {
	// Get the texture from the shader resource view
	ComPtr<ID3D11Texture2D> tex2d;
	GetTexture2D(device, texture, tex2d.GetAddressOf());

	// Convert the texture to an image
	return CaptureTexture(&device, &device_context, tex2d.Get(), output);
}


export namespace render::exporter {

bool ExportTextureToDDS(ID3D11Device& device, ID3D11DeviceContext& device_context, const Texture& texture, fs::path filename) {
	// Convert the texture to an image
	ScratchImage image;
	const HRESULT convert = ConvertToScratchImage(device, device_context, texture, image);

	// Save the image
	if (SUCCEEDED(convert)) {
		filename.replace_extension("dds");

		const HRESULT save = SaveToDDSFile(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DDS_FLAGS_NONE, filename.c_str());
		if (FAILED(save)) {
			Logger::log(LogLevel::err, "Error exporting texture to file {} - Failed to save image", filename.string());
			return false;
		}
	}
	else {
		Logger::log(LogLevel::err, "Error exporting texture to file {} - Failed to convert texture to image", filename.string());
		return false;
	}

	return true;
}

bool ExportTextureToTGA(ID3D11Device& device, ID3D11DeviceContext& device_context, const Texture& texture, fs::path filename) {
	// Convert the texture to an image
	ScratchImage image;
	const HRESULT convert = ConvertToScratchImage(device, device_context, texture, image);

	// Save the image
	if (SUCCEEDED(convert)) {
		filename.replace_extension("tga");

		const HRESULT save = SaveToTGAFile(*image.GetImage(0, 0, 0), filename.c_str());
		if (FAILED(save)) {
			Logger::log(LogLevel::err, "Error exporting texture to file {} - Failed to save image", filename.string());
			return false;
		}
	}
	else {
		Logger::log(LogLevel::err, "Error exporting texture to file {} - Failed to convert texture to image", filename.string());
		return false;
	}

	return true;
}

bool ExportTextureToWIC(ID3D11Device& device, ID3D11DeviceContext& device_context, const Texture& texture, fs::path filename, DirectX::WICCodecs codec) {
	// Convert the texture to an image
	ScratchImage image;
	const HRESULT convert = ConvertToScratchImage(device, device_context, texture, image);

	// Save the image
	if (SUCCEEDED(convert)) {
		switch (codec) {
			case WICCodecs::WIC_CODEC_BMP:
				filename.replace_extension("bmp");
				break;
			case WICCodecs::WIC_CODEC_GIF:
				filename.replace_extension("gif");
				break;
			case WICCodecs::WIC_CODEC_ICO:
				filename.replace_extension("ico");
				break;
			case WICCodecs::WIC_CODEC_JPEG:
				filename.replace_extension("jpeg");
				break;
			case WICCodecs::WIC_CODEC_PNG:
				filename.replace_extension("png");
				break;
			case WICCodecs::WIC_CODEC_TIFF:
				filename.replace_extension("tiff");
				break;
			case WICCodecs::WIC_CODEC_WMP:
				filename.replace_extension("wmp");
				break;
		}

		const HRESULT save = SaveToWICFile(*image.GetImage(0, 0, 0), WIC_FLAGS_NONE, GetWICCodec(codec), filename.c_str());
		if (FAILED(save)) {
			Logger::log(LogLevel::err, "Error exporting texture to file {} - Failed to save image", filename.string());
			return false;
		}
	}
	else {
		Logger::log(LogLevel::err, "Error exporting texture to file {} - Failed to convert texture to image", filename.string());
		return false;
	}
	
	return true;
}

} //namespace render::exporter
