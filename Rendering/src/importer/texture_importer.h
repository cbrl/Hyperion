#pragma once

#include "directx/d3d11.h"
#include "directx/directxtk.h"
#include "datatypes/datatypes.h"


namespace render::importer {

// Load a texture from a file (jpg, png, etc...)
void ImportTexture(ID3D11Device& device,
	               ID3D11DeviceContext& device_context,
                   const std::wstring& filename,
                   ID3D11ShaderResourceView** srv_out);

// Load a Texture2DArray from multiple files
void ImportTexture(ID3D11Device& device,
	               ID3D11DeviceContext& device_context,
                   const std::vector<std::wstring>& filenames,
                   ID3D11ShaderResourceView** srv_out);

} //namespace render::importer
