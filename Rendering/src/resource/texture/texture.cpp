#include "texture.h"
#include "loader/texture_loader.h"


namespace render {

Texture::Texture(ID3D11Device& device,
                 ID3D11DeviceContext& device_context,
                 const std::wstring& filename)
	: Resource(filename) {

	TextureLoader::load(device, device_context, filename, texture_srv.GetAddressOf());
}


Texture::Texture(const std::wstring& guid,
                 ID3D11Device& device,
                 const D3D11_TEXTURE2D_DESC& desc,
                 const D3D11_SUBRESOURCE_DATA& init_data)
	: Resource(guid) {

	HRESULT hr;
	ComPtr<ID3D11Texture2D> texture;

	hr = device.CreateTexture2D(&desc,
	                            &init_data,
	                            texture.ReleaseAndGetAddressOf());
	ThrowIfFailed(hr, "Failed to create Texture2D");

	hr = device.CreateShaderResourceView(texture.Get(),
	                                     nullptr,
	                                     texture_srv.ReleaseAndGetAddressOf());
	ThrowIfFailed(hr, "Failed to create Texture SRV");
}

} //namespace render