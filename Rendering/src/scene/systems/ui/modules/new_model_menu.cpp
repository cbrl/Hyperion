#include "new_model_menu.h"

#include "imgui.h"
#include "entity/entity.h"
#include "resource/resource_mgr.h"
#include "scene/scene.h"
#include "resource/model/blueprint_factory.h"


void NewModelMenu::drawMenu() {
	if (ImGui::MenuItem("Cube"))
		model_type = ModelType::Cube;
	if (ImGui::MenuItem("Box"))
		model_type = ModelType::Box;
	if (ImGui::MenuItem("Sphere"))
		model_type = ModelType::Sphere;
	if (ImGui::MenuItem("GeoSphere"))
		model_type = ModelType::GeoSphere;
	if (ImGui::MenuItem("Cylinder"))
		model_type = ModelType::Cylinder;
	if (ImGui::MenuItem("Cone"))
		model_type = ModelType::Cone;
	if (ImGui::MenuItem("Torus"))
		model_type = ModelType::Torus;
	if (ImGui::MenuItem("Tetrahedron"))
		model_type = ModelType::Tetrahedron;
	if (ImGui::MenuItem("Octahedron"))
		model_type = ModelType::Octahedron;
	if (ImGui::MenuItem("Dodecahedron"))
		model_type = ModelType::Dodecahedron;
	if (ImGui::MenuItem("Icosahedron"))
		model_type = ModelType::Icosahedron;
}



void NewModelMenu::procNewModelPopup(ID3D11Device& device,
                                     render::ResourceMgr& resource_mgr,
                                     render::Scene& scene,
                                     ecs::EntityPtr entity) {

	using namespace render;

	// Open popup
	switch (model_type) {
		case ModelType::Cube:
			ImGui::OpenPopup("New Cube");
			break;
		case ModelType::Box:
			ImGui::OpenPopup("New Box");
			break;
		case ModelType::Sphere:
			ImGui::OpenPopup("New Sphere");
			break;
		case ModelType::GeoSphere:
			ImGui::OpenPopup("New GeoSphere");
			break;
		case ModelType::Cylinder:
			ImGui::OpenPopup("New Cylinder");
			break;
		case ModelType::Torus:
			ImGui::OpenPopup("New Torus");
			break;
		case ModelType::Cone:
			ImGui::OpenPopup("New Cone");
			break;
		case ModelType::Tetrahedron:
			ImGui::OpenPopup("New Tetrahedron");
			break;
		case ModelType::Octahedron:
			ImGui::OpenPopup("New Octahedron");
			break;
		case ModelType::Dodecahedron:
			ImGui::OpenPopup("New Dodecahedron");
			break;
		case ModelType::Icosahedron:
			ImGui::OpenPopup("New Icosahedron");
			break;
		default:
			break;
	}


	// Reset model type
	model_type = ModelType::None;


	if (ImGui::BeginPopupModal("New Cube", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 size = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateCube(resource_mgr, config, size);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Box", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32_3 size = {1.0f, 1.0f, 1.0f};
		static bool flip_winding = false;

		ImGui::InputFloat3("Size", size.data());
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateBox(resource_mgr, config, size);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Sphere", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 diameter = 1.0f;
		static size_t tessellation = 16;
		static bool flip_winding = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 64) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateSphere(resource_mgr, config, diameter, tessellation);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New GeoSphere", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 diameter = 1.0f;
		static size_t tessellation = 3;
		static bool flip_winding = false;

		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 6) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is extremely high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateGeoSphere(resource_mgr, config, diameter, tessellation);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Cylinder", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 height = 1.0f;
		static f32 diameter = 1.0f;
		static size_t tessellation = 32;
		static bool flip_winding = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCylinder(resource_mgr, config, diameter, height, tessellation);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Cone", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 height = 1.0f;
		static f32 diameter = 1.0f;
		static size_t tessellation = 32;
		static bool flip_winding = false;

		ImGui::InputFloat("Height", &height);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateCone(resource_mgr, config, diameter, height, tessellation);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Torus", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 thickness = 0.333f;
		static f32 diameter = 1.0f;
		static size_t tessellation = 32;
		static bool flip_winding = false;

		ImGui::InputFloat("Thickness", &thickness);
		ImGui::InputFloat("Diameter", &diameter);
		ImGui::InputInt("Tessellation", (int*)&tessellation);
		ImGui::Checkbox("Flip winding", &flip_winding);

		ImGui::Button("Create");

		if (ImGui::IsItemHovered() && tessellation >= 96) {
			ImGui::BeginTooltip();
			ImGui::Text("WARNING: Tessellation amount is very high");
			ImGui::EndTooltip();
		}

		if (ImGui::IsItemClicked()) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			if (tessellation < 3) tessellation = 3;
			auto bp = BlueprintFactory::CreateTorus(resource_mgr, config, diameter, thickness, tessellation);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Tetrahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 size = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateTetrahedron(resource_mgr, config, size);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Octahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 size = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateOctahedron(resource_mgr, config, size);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Dodecahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 size = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateDodecahedron(resource_mgr, config, size);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New Icosahedron", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		static f32 size = 1.0f;
		static bool flip_winding = false;

		ImGui::InputFloat("Size", &size);
		ImGui::Checkbox("Flip winding", &flip_winding);

		if (ImGui::Button("Create")) {
			ModelConfig<VertexPositionNormalTexture> config;
			config.flip_winding = flip_winding;
			auto bp = BlueprintFactory::CreateIcosahedron(resource_mgr, config, size);
			if (entity) scene.importModel(entity, device, bp);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}