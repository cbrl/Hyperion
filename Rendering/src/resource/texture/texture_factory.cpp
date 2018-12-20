#include "texture_factory.h"
#include "resource/resource_mgr.h"


namespace TextureFactory {

std::shared_ptr<Texture> CreateDefaultTexture(ResourceMgr& resource_mgr) {

	u32 tex_data[128][128];

	constexpr u32 color  = 0xFFFF0000;
	constexpr u32 color2 = 0xFF00FFFF;

	constexpr auto x_size = std::size(tex_data[0]);
	constexpr auto y_size = std::size(tex_data);

	constexpr auto x_half_size = x_size / 2;
	constexpr auto y_half_size = y_size / 2;

	for (size_t i = 0; i < y_half_size; ++i) {
		for (size_t j = 0; j < x_half_size; ++j) {
			tex_data[i][j] = color;
		}
		for (size_t j = x_half_size; j < x_size; ++j) {
			tex_data[i][j] = color2;
		}
	}

	for (size_t i = y_half_size; i < y_size; ++i) {
		for (size_t j = 0; j < x_half_size; ++j) {
			tex_data[i][j] = color2;
		}
		for (size_t j = x_half_size; j < x_size; ++j) {
			tex_data[i][j] = color;
		}
	}

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem     = tex_data;
	init_data.SysMemPitch = sizeof(u32) * 128;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width            = 128;
	desc.Height           = 128;
	desc.MipLevels        = 1;
	desc.ArraySize        = 1;
	desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage            = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;


	return resource_mgr.getOrCreate<Texture>(L"Default", desc, init_data);
}


std::shared_ptr<Texture> CreateColorTexture(ResourceMgr& resource_mgr, const vec4_f32& color) {

	u32 color_u32 = Float4ColorToU32(color);

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem     = &color_u32;
	init_data.SysMemPitch = sizeof(u32);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width            = desc.Height = desc.MipLevels = desc.ArraySize = 1;
	desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage            = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;


	return resource_mgr.getOrCreate<Texture>(std::to_wstring(color_u32), desc, init_data);
}

}