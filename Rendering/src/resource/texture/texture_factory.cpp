#include "texture_factory.h"
#include "resource/resource_mgr.h"


namespace TextureFactory {

shared_ptr<Texture> CreateDefaultTexture(ResourceMgr& resource_mgr) {

	u32 tex_data[128][128];

	u32 color = 0xFFFF0000;
	for (size_t i = 0; i < 128; ++i) {
		if (i > 64) color = 0xFF00FFFF;
		for (size_t j = 0; j < 128; ++j) {
			if (j > 64) tex_data[i][j] = color ^ 0x00FFFFFF;
			else tex_data[i][j] = color;
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


shared_ptr<Texture> CreateColorTexture(ResourceMgr& resource_mgr, const vec4_f32& color) {

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