module;

#include "io/io.h"

export module rendering.exporter.model_exporter;

export import rendering.model_blueprint;
import rendering.exporter.assimp_exporter;

struct ID3D11Device;
struct ID3D11DeviceContext;

export namespace render::exporter {

void ExportModel(ID3D11Device& device, ID3D11DeviceContext& device_context, const ModelBlueprint& blueprint, const fs::path& filename) {
	detail::AssimpExport(device, device_context, blueprint, filename);
}

} // namespace render::exporter
