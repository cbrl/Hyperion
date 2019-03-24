#include "system_menu.h"
#include "engine/engine.h"

#include "imgui.h"


SystemMenu::SystemMenu(const Engine& engine) {

	auto& rendering_mgr = engine.getRenderingMgr();
	auto& settings      = rendering_mgr.getDisplayConfig();
	smap_res            = rendering_mgr.getRenderingConfig().getShadowMapRes();

	// Create the display mode strings
	for (const auto& desc : settings.getDisplayDescList()) {
		const auto exact_refresh = static_cast<f32>(desc.RefreshRate.Numerator) / static_cast<f32>(desc.RefreshRate.Denominator);
		const auto refresh = static_cast<u32>(round(exact_refresh));
		display_modes.push_back(std::to_string(desc.Width) + "x" + std::to_string(desc.Height) + " " + std::to_string(refresh) + "Hz");
	}
}


void SystemMenu::draw(Engine& engine) {

	bool display_settings_popup = false;
	bool engine_settings_popup = false;

	// Menu Bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("System")) {

			if (ImGui::MenuItem("Display Settings")) {
				display_settings_popup = true;
			}

			if (ImGui::MenuItem("Engine Settings")) {
				engine_settings_popup = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit")) {
				engine.requestExit();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Text Editor")) {
			if (ImGui::MenuItem("Open text editor")) {
				text_editor = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	if (display_settings_popup) {
		ImGui::OpenPopup("Display Settings");
	}
	if (engine_settings_popup) {
		ImGui::OpenPopup("Engine Settings");
	}


	// Settings Popups
	drawDisplaySettingsPopup(engine);
	drawEngineSettingsPopup(engine);
}


bool& SystemMenu::getTextEditorState() noexcept {
	return text_editor;
}


void SystemMenu::drawEngineSettingsPopup(Engine& engine) {

	if (ImGui::BeginPopupModal("Engine Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		auto& rendering_mgr    = engine.getRenderingMgr();
		auto& rendering_config = rendering_mgr.getRenderingConfig();

		ImGui::DragScalar("Shadow Map Resolution", ImGuiDataType_U32, &smap_res, 1);

		if (ImGui::Button("Apply")) {
			rendering_config.setShadowMapRes(smap_res);
			engine.saveConfig();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			rendering_config.setShadowMapRes(smap_res);
			engine.saveConfig();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}


void SystemMenu::drawDisplaySettingsPopup(Engine& engine) {

	if (ImGui::BeginPopupModal("Display Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		auto& settings = engine.getRenderingMgr().getDisplayConfig();

		// Lamda to convert display mode to a string
		static const auto getter = [](void* data, int idx, const char** out_text) -> bool {
			auto& vector = *static_cast<const std::vector<std::string>*>(data);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
			*out_text = vector[idx].c_str();
			return true;
		};

		ImGui::Combo("Display Mode",
		             &current_display_desc,
		             getter,
		             &display_modes,
		             static_cast<int>(display_modes.size()));


		if (ImGui::Button("Apply")) {
			settings.setDisplayDesc(current_display_desc);
			engine.saveConfig();
			engine.requestResize();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save")) {
			settings.setDisplayDesc(current_display_desc);
			engine.saveConfig();
			engine.requestResize();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
