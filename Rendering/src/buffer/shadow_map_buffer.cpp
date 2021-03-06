#include "shadow_map_buffer.h"


namespace render {

//----------------------------------------------------------------------------------
// ShadowMapBuffer
//----------------------------------------------------------------------------------

ShadowMapBuffer::ShadowMapBuffer(ID3D11Device& device,
                                 u32 map_count,
                                 u32 resolution,
                                 i32 depth_bias,
                                 f32 slope_scaled_depth_bias,
                                 f32 depth_bias_clamp) {

	init(device, map_count, resolution, depth_bias, slope_scaled_depth_bias, depth_bias_clamp);
}

void ShadowMapBuffer::init(ID3D11Device& device,
                           u32 map_count,
                           u32 resolution,
                           i32 depth_bias,
                           f32 slope_scaled_depth_bias,
                           f32 depth_bias_clamp) {

	ThrowIfFailed(resolution != 0, "ShadowMapBuffer resolution must be greater than 0");

	// Create the raster state
	D3D11_RASTERIZER_DESC raster_desc = {};
	raster_desc.CullMode              = D3D11_CULL_BACK;
	raster_desc.FillMode              = D3D11_FILL_SOLID;
	raster_desc.DepthBias             = depth_bias;
	raster_desc.SlopeScaledDepthBias  = slope_scaled_depth_bias;
	raster_desc.DepthBiasClamp        = depth_bias_clamp;
	raster_desc.DepthClipEnable       = TRUE;
	raster_desc.MultisampleEnable     = TRUE;

	ThrowIfFailed(device.CreateRasterizerState(&raster_desc, raster_state.GetAddressOf()),
	              "Failed to create the raster state for a shadow map buffer");


	// Define the viewport
	viewport.setTopLeft(0, 0);
	viewport.setSize(resolution, resolution);
	viewport.setDepth(0.0f, 1.0f);


	// Create the depth map texture
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width                = resolution;
	tex_desc.Height               = resolution;
	tex_desc.MipLevels            = 1;
	tex_desc.ArraySize            = map_count;
	tex_desc.Format               = DXGI_FORMAT_R16_TYPELESS;
	tex_desc.SampleDesc.Count     = 1;
	tex_desc.SampleDesc.Quality   = 0;
	tex_desc.Usage                = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags            = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags       = 0;
	tex_desc.MiscFlags            = 0;

	ComPtr<ID3D11Texture2D> depth_map;
	ThrowIfFailed(device.CreateTexture2D(&tex_desc, nullptr, depth_map.GetAddressOf()),
	              "Failed to create the depth map texture for a shadow map buffer");


	// Create the depth stencil views
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Flags                         = 0;
	dsv_desc.Format                        = DXGI_FORMAT_D16_UNORM;
	dsv_desc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsv_desc.Texture2DArray.MipSlice       = 0;
	dsv_desc.Texture2DArray.ArraySize      = 1;

	dsvs.clear();
	dsvs.resize(tex_desc.ArraySize);

	for (size_t i = 0; i < tex_desc.ArraySize; ++i) {
		dsv_desc.Texture2DArray.FirstArraySlice = static_cast<u32>(i);

		ThrowIfFailed(device.CreateDepthStencilView(depth_map.Get(), &dsv_desc, dsvs[i].GetAddressOf()),
		              "Failed to create a depth stencil view for a shadow map buffer");
	}


	// Create the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	srv_desc.Format                          = DXGI_FORMAT_R16_UNORM;
	srv_desc.ViewDimension                   = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MipLevels        = tex_desc.MipLevels;
	srv_desc.Texture2DArray.MostDetailedMip  = 0;
	srv_desc.Texture2DArray.ArraySize        = tex_desc.ArraySize;

	ThrowIfFailed(device.CreateShaderResourceView(depth_map.Get(), &srv_desc, srv.GetAddressOf()),
	              "Failed to create the shader resource view for a shadow map buffer");
}



//----------------------------------------------------------------------------------
// ShadowCubeMapBuffer
//----------------------------------------------------------------------------------

ShadowCubeMapBuffer::ShadowCubeMapBuffer(ID3D11Device& device,
                                         u32 cube_map_count,
                                         u32 resolution,
                                         i32 depth_bias,
                                         f32 slope_scaled_depth_bias,
                                         f32 depth_bias_clamp) {
	
	init(device, cube_map_count, resolution, depth_bias, slope_scaled_depth_bias, depth_bias_clamp);
}


void ShadowCubeMapBuffer::init(ID3D11Device& device,
                               u32 cube_map_count,
                               u32 resolution,
                               i32 depth_bias,
                               f32 slope_scaled_depth_bias,
                               f32 depth_bias_clamp) {

	ThrowIfFailed(resolution != 0, "ShadowCubeMapBuffer resolution must be greater than 0");

	// Create the raster state
	D3D11_RASTERIZER_DESC raster_desc = {};
	raster_desc.CullMode              = D3D11_CULL_BACK;
	raster_desc.FillMode              = D3D11_FILL_SOLID;
	raster_desc.DepthBias             = depth_bias;
	raster_desc.SlopeScaledDepthBias  = slope_scaled_depth_bias;
	raster_desc.DepthBiasClamp        = depth_bias_clamp;
	raster_desc.DepthClipEnable       = TRUE;
	raster_desc.MultisampleEnable     = TRUE;

	ThrowIfFailed(device.CreateRasterizerState(&raster_desc, raster_state.GetAddressOf()),
	              "Failed to create the raster state for a shadow cube map buffer");


	// Define the viewport
	viewport.setTopLeft(0, 0);
	viewport.setSize(resolution, resolution);
	viewport.setDepth(0.0f, 1.0f);


	// Create the depth map texture
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width                = resolution;
	tex_desc.Height               = resolution;
	tex_desc.MipLevels            = 1;
	tex_desc.ArraySize            = cube_map_count * 6;
	tex_desc.Format               = DXGI_FORMAT_R16_TYPELESS;
	tex_desc.SampleDesc.Count     = 1;
	tex_desc.SampleDesc.Quality   = 0;
	tex_desc.Usage                = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags            = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.MiscFlags            = D3D11_RESOURCE_MISC_TEXTURECUBE;
	tex_desc.CPUAccessFlags       = 0;

	ComPtr<ID3D11Texture2D> depth_map;
	ThrowIfFailed(device.CreateTexture2D(&tex_desc, nullptr, depth_map.GetAddressOf()),
	              "Failed to create the depth map texture for a shadow cube map buffer");


	// Create the depth stencil views
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Flags                         = 0;
	dsv_desc.Format                        = DXGI_FORMAT_D16_UNORM;
	dsv_desc.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsv_desc.Texture2DArray.MipSlice       = 0;
	dsv_desc.Texture2DArray.ArraySize      = 1;

	dsvs.clear();
	dsvs.resize(tex_desc.ArraySize);

	for (size_t i = 0; i < tex_desc.ArraySize; ++i) {
		dsv_desc.Texture2DArray.FirstArraySlice = static_cast<u32>(i);

		ThrowIfFailed(device.CreateDepthStencilView(depth_map.Get(), &dsv_desc, dsvs[i].GetAddressOf()),
		              "Failed to create a depth stencil view for a shadow cube map buffer");
	}


	// Create the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc  = {};
	srv_desc.Format                           = DXGI_FORMAT_R16_UNORM;
	srv_desc.ViewDimension                    = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
	srv_desc.TextureCubeArray.MipLevels       = tex_desc.MipLevels;
	srv_desc.TextureCubeArray.MostDetailedMip = 0;
	srv_desc.TextureCubeArray.NumCubes        = cube_map_count;

	ThrowIfFailed(device.CreateShaderResourceView(depth_map.Get(), &srv_desc, srv.GetAddressOf()),
	              "Failed to create the shader resource view for a shadow cube map buffer");
}

} //namespace render