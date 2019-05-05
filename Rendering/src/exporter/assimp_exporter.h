#pragma once

#include "io/io.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace render {

class ModelBlueprint;

namespace exporter::detail {

void AssimpExport(ID3D11Device& device, ID3D11DeviceContext& device_context, const ModelBlueprint& blueprint, fs::path filename);

} //namespace exporter::detail

} //namespace render