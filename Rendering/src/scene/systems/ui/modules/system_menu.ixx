module;

#include "datatypes/types.h"
#include "os/windows/win_utils.h"
#include <DirectXTex.h>

#include "imgui.h"

export module systems.user_interface.modules.system_menu;

import rendering.engine;
import rendering.exporter.model_exporter;
import rendering.exporter.texture_exporter;


export class SystemMenu final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SystemMenu(const Engine& engine) {
		const auto& rendering_mgr    = engine.getRenderingMgr();
		const auto& rendering_config = rendering_mgr.getRenderingConfig();
		const auto& display_config   = rendering_mgr.getDisplayConfig();

		// Get shadow map config
		smap_res = rendering_config.getShadowMapRes();
		smap_depth_bias = rendering_config.getShadowMapDepthBias();
		smap_slope_scaled_depth_bias = rendering_config.getShadowMapSlopeScaledDepthBias();
		smap_depth_bias_clamp = rendering_config.getShadowMapDepthBiasClamp();

		// Create the display mode strings
		for (const auto& desc : display_config.getDisplayDescList()) {
			const auto exact_refresh = static_cast<f32>(desc.RefreshRate.Numerator) / static_cast<f32>(desc.RefreshRate.Denominator);
			const auto refresh = static_cast<u32>(round(exact_refresh));
			display_modes.push_back(std::to_string(desc.Width) + "x" + std::to_string(desc.Height) + " " + std::to_string(refresh) + "Hz");
		}
	}

	SystemMenu(const SystemMenu&) = default;
	SystemMenu(SystemMenu&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SystemMenu() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SystemMenu& operator=(const SystemMenu&) = default;
	SystemMenu& operator=(SystemMenu&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void draw(Engine& engine) {
		bool display_settings_popup = false;
		bool engine_settings_popup  = false;
		bool export_model_popup     = false;
		bool export_texture_popup   = false;

		// Menu Bar
		if (ImGui::BeginMainMenuBar()) {

			// System
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

			// Text Editor
			if (ImGui::BeginMenu("Text Editor")) {
				if (ImGui::MenuItem("Open text editor")) {
					text_editor = true;
				}
				ImGui::EndMenu();
			}

			// Export
			if (ImGui::BeginMenu("Export")) {
				if (ImGui::MenuItem("Model")) {
					export_model_popup = true;
				}
				if (ImGui::MenuItem("Texture")) {
					export_texture_popup = true;
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
		if (export_model_popup) {
			ImGui::OpenPopup("Export Model");
		}
		if (export_texture_popup) {
			ImGui::OpenPopup("Export Texture");
		}

		// Settings Popups
		drawDisplaySettingsPopup(engine);
		drawEngineSettingsPopup(engine);
		drawExportModelPopup(engine);
		drawExportTexturePopup(engine);
	}

	[[nodiscard]]
	bool& getTextEditorState() noexcept {
		return text_editor;
	}

private:

	void drawEngineSettingsPopup(Engine& engine) {
		if (ImGui::BeginPopupModal("Engine Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

			auto& rendering_mgr = engine.getRenderingMgr();
			auto& rendering_config = rendering_mgr.getRenderingConfig();

			ImGui::Text("Shadow Maps");
			ImGui::Separator();

			ImGui::DragScalar("Resolution", ImGuiDataType_U32, &smap_res, 1);
			ImGui::DragScalar("Depth Bias", ImGuiDataType_S32, &smap_depth_bias, 1);
			ImGui::DragFloat("Slope Scaled Depth Bias", &smap_slope_scaled_depth_bias, 0.01f);
			ImGui::DragFloat("Depth Bias Clamp", &smap_depth_bias_clamp, 0.01f);

			bool apply = false;
			bool save = false;
			bool close = false;

			apply = ImGui::Button("Apply");
			ImGui::SameLine();
			save = ImGui::Button("Save");
			ImGui::SameLine();
			close = ImGui::Button("Close");

			if (apply || save) {
				rendering_config.setShadowMapRes(smap_res);
				rendering_config.setShadowMapDepthBias(smap_depth_bias);
				rendering_config.setShadowMapSlopeScaledDepthBias(smap_slope_scaled_depth_bias);
				rendering_config.setShadowMapDepthBiasClamp(smap_depth_bias_clamp);
				engine.saveConfig();

				if (save)
					ImGui::CloseCurrentPopup();
			}
			if (close) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void drawDisplaySettingsPopup(Engine& engine) {
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

	void drawExportModelPopup(Engine& engine) {
		ImGui::SetNextWindowContentWidth(400);
		if (ImGui::BeginPopup("Export Model", ImGuiWindowFlags_NoResize)) {
			auto& rendering_mgr  = engine.getRenderingMgr();
			auto& device         = rendering_mgr.getDevice();
			auto& device_context = rendering_mgr.getDeviceContext();
			auto& resource_mgr   = rendering_mgr.getResourceMgr();

			const auto& blueprints = resource_mgr.getResourceMap<render::ModelBlueprint>();

			for (const auto& [name, blueprint_weakptr] : blueprints) {
				if (!blueprint_weakptr)
					continue;

				ImGui::Text(blueprint_weakptr->getGUID().c_str());

				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 40);

				if (ImGui::Button("Export")) {
					const fs::path path = SaveFileDialog();
					if (not path.empty()) {
						render::exporter::ExportModel(device, device_context, *blueprint_weakptr, path);
					}
				}

				ImGui::Separator();
				ImGui::Spacing();
			}

			ImGui::Spacing();
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void drawExportTexturePopup(Engine& engine) {
		ImGui::SetNextWindowContentWidth(500);
		if (ImGui::BeginPopup("Export Texture", ImGuiWindowFlags_AlwaysAutoResize)) {
			auto& rendering_mgr  = engine.getRenderingMgr();
			auto& device         = rendering_mgr.getDevice();
			auto& device_context = rendering_mgr.getDeviceContext();
			auto& resource_mgr   = rendering_mgr.getResourceMgr();

			const auto& textures = resource_mgr.getResourceMap<render::Texture>();

			for (const auto& [name, texture_weakptr] : textures) {
				if (!texture_weakptr)
					continue;

				// Draw texture
				ImGui::Image(texture_weakptr->get(), {40, 40});

				ImGui::SameLine();
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

				// Texture name
				ImGui::Text(texture_weakptr->getGUID().c_str());

				// Move cursor to right side of window
				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 40);
			
				// Push ID for upcoming popup
				ImGui::PushID(&name);

				if (ImGui::Button("Export")) {
					ImGui::OpenPopup("FileType");
				}
				if (ImGui::BeginPopup("FileType")) {
					using namespace DirectX;

					if (ImGui::MenuItem("DDS")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToDDS(device, device_context, *texture_weakptr, file);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("TGA")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToTGA(device, device_context, *texture_weakptr, file);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("PNG")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToWIC(device, device_context, *texture_weakptr, file, WICCodecs::WIC_CODEC_PNG);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("TIFF")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToWIC(device, device_context, *texture_weakptr, file, WICCodecs::WIC_CODEC_TIFF);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("BMP")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToWIC(device, device_context, *texture_weakptr, file, WICCodecs::WIC_CODEC_BMP);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("JPEG")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToWIC(device, device_context, *texture_weakptr, file, WICCodecs::WIC_CODEC_JPEG);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("GIF")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToWIC(device, device_context, *texture_weakptr, file, WICCodecs::WIC_CODEC_GIF);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("WMP")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToWIC(device, device_context, *texture_weakptr, file, WICCodecs::WIC_CODEC_WMP);
					}
					ImGui::Separator();
					if (ImGui::MenuItem("ICO")) {
						const fs::path file = SaveFileDialog();
						if (not file.empty())
							render::exporter::ExportTextureToWIC(device, device_context, *texture_weakptr, file, WICCodecs::WIC_CODEC_ICO);
					}

					ImGui::EndPopup();
				}

				// Pop item ID
				ImGui::PopID();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 12);
				ImGui::Separator();
			}

			ImGui::Spacing();
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Text editor visibility
	bool text_editor = false;

	// Display modes
	int current_display_desc = 0;
	std::vector<std::string> display_modes;

	// Shadow map variables
	u32 smap_res = 0;
	i32 smap_depth_bias = 0;
	f32 smap_slope_scaled_depth_bias = 0;
	f32 smap_depth_bias_clamp = 0;
};