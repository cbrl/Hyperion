#include "texture_factory.h"
#include "resource/resource_mgr.h"


namespace TextureFactory {

std::shared_ptr<Texture> CreateDefaultTexture(ResourceMgr& resource_mgr) {

	static u32 tex_data[128][128] = {{}};
	static bool initialized = false;

	if (!initialized) {
		constexpr u32 color  = 0xFF808080; //ABGR
		constexpr u32 color2 = 0xFFA0A0A0;

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

		initialized = true;
	}

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem     = tex_data;
	init_data.SysMemPitch = static_cast<UINT>(sizeof(u32) * std::size(tex_data[0]));

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width            = std::size(tex_data[0]);
	desc.Height           = std::size(tex_data);
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


std::shared_ptr<Texture> CreateWhiteTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {1.0f, 1.0f, 1.0f, 1.0f});
}


std::shared_ptr<Texture> CreateBlackTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {0.0f, 0.0f, 0.0f, 1.0f});
}


std::shared_ptr<Texture> CreateRedTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {1.0f, 0.0f, 0.0f, 1.0f});
}


std::shared_ptr<Texture> CreateGreenTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {0.0f, 1.0f, 0.0f, 1.0f});
}


std::shared_ptr<Texture> CreateBlueTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {0.0f, 0.0f, 1.0f, 1.0f});
}


std::shared_ptr<Texture> CreateCyanTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {0.0f, 1.0f, 1.0f, 1.0f});
}


std::shared_ptr<Texture> CreateMagentaTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {1.0f, 0.0f, 1.0f, 1.0f});
}


std::shared_ptr<Texture> CreateYellowTexture(ResourceMgr& resource_mgr) {
	return CreateColorTexture(resource_mgr, {1.0f, 1.0f, 0.0f, 1.0f});
}

}