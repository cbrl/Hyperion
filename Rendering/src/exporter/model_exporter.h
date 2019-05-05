#pragma once

#include "io/io.h"
#include "entity/entity_ptr.h"
#include "exporter/assimp_exporter.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace render {
	
class ModelBlueprint;

namespace exporter {

void ExportModel(ID3D11Device& device, ID3D11DeviceContext& device_context, const ModelBlueprint& blueprint, const fs::path& filename) {
	detail::AssimpExport(device, device_context, blueprint, filename);
}

} // namespace exporter

} //namespace render