#pragma once

#include "io/io.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
namespace DirectX { enum WICCodecs; }

namespace render {

class Texture;

namespace exporter {

bool ExportTextureToDDS(ID3D11Device& device, ID3D11DeviceContext& device_context, const Texture& texture, fs::path filename);

bool ExportTextureToTGA(ID3D11Device& device, ID3D11DeviceContext& device_context, const Texture& texture, fs::path filename);

bool ExportTextureToWIC(ID3D11Device& device, ID3D11DeviceContext& device_context, const Texture& texture, fs::path filename, DirectX::WICCodecs);

} //namespace exporter
} //namespace render