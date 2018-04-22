#include "stdafx.h"
#include "shadow_map_buffer.h"

ShadowMapBuffer::ShadowMapBuffer(ID3D11Device* device,
								 u32 map_count,
								 u32 width,
								 u32 height) {

	// Define the viewport
	viewport.Width    = static_cast<float>(width);
	viewport.Height   = static_cast<float>(height);
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;


	// Create the depth map texture. Typeless format is used because
	// the DSV and SRV interpret the bits as D24_UNORM_S8_UINT and
	// R24_UNORM_X8_TYPELESS respectively.
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width              = width;
	tex_desc.Height             = height;
	tex_desc.MipLevels          = 1;
	tex_desc.ArraySize          = 1;
	tex_desc.Format             = DXGI_FORMAT_R24G8_TYPELESS;
	tex_desc.SampleDesc.Count   = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage              = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags     = 0;
	tex_desc.MiscFlags          = 0;

	ComPtr<ID3D11Texture2D> depth_map;
	ThrowIfFailed(device->CreateTexture2D(&tex_desc, 0, depth_map.GetAddressOf()),
				  "Failed to create the depth map texture for a shadow map buffer");


	// Create the depth stencil views
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Flags              = 0;
	dsv_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;

	dsvs.resize(map_count);

	for (u32 i = 0; i < map_count; ++i) {
		ThrowIfFailed(device->CreateDepthStencilView(depth_map.Get(), &dsv_desc, &dsvs[i]),
		              "Failed to create a depth stencil view for a shadow map buffer");
	}
	

	// Create the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format                    = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srv_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels       = tex_desc.MipLevels;
	srv_desc.Texture2D.MostDetailedMip = 0;

	ThrowIfFailed(device->CreateShaderResourceView(depth_map.Get(), &srv_desc, srv.GetAddressOf()),
				  "Failed to create the shader resource view for a shadow map buffer");
}