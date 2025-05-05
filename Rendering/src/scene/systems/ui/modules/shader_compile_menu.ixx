module;

#include "datatypes/types.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

export module rendering:systems.user_interface.modules.shader_compile_menu;

import :engine;
import :rendering_mgr;
import :resource_mgr;
import :shader;
import :shader_factory;


export class ShaderCompileMenu final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShaderCompileMenu() = default;
	ShaderCompileMenu(const ShaderCompileMenu&) = default;
	ShaderCompileMenu(ShaderCompileMenu&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ShaderCompileMenu() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ShaderCompileMenu& operator=(const ShaderCompileMenu&) = default;
	ShaderCompileMenu& operator=(ShaderCompileMenu&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Draw the shader menu. Must be called in a context where ImGui::BeginMenu() is valid.
	void drawMenu() {
		if (ImGui::BeginMenu("Shader")) {
			if (ImGui::MenuItem("Compile Shader")) {
				compile_popup_open = true;
			}
			ImGui::EndMenu();
		}
	}

	// Call every frame to render the compile popup if it is open.
	// Returns true if "Compile" was clicked, and thus compileShader() needs to be called.
	[[nodiscard]]
	bool update(render::Engine& engine) {
		return drawCompilePopup(engine);
	}

	// Compile an input shader of the type selected in this menu
	void compileShader(render::Engine& engine, std::string_view data) const {
		using namespace render;
		using ShaderFactory::CreateShaderFromMemory;

		auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
		const std::string target_ver = shader_types[type_idx].second;

		if (checkTypeSelected<ComputeShader>()) {
			(void)CreateShaderFromMemory<ComputeShader>(resource_mgr,
														data,
														StrToWstr(shader_name),
														entry_point,
														target_ver);
		}
		else if (checkTypeSelected<DomainShader>()) {
			(void)CreateShaderFromMemory<DomainShader>(resource_mgr,
													   data,
													   StrToWstr(shader_name),
													   entry_point,
													   target_ver);
		}
		else if (checkTypeSelected<GeometryShader>()) {
			(void)CreateShaderFromMemory<GeometryShader>(resource_mgr,
														 data,
														 StrToWstr(shader_name),
														 entry_point,
														 target_ver);
		}
		else if (checkTypeSelected<HullShader>()) {
			(void)CreateShaderFromMemory<HullShader>(resource_mgr,
													 data,
													 StrToWstr(shader_name),
													 entry_point,
													 target_ver);
		}
		else if (checkTypeSelected<PixelShader>()) {
			(void)CreateShaderFromMemory<PixelShader>(resource_mgr,
													  data,
													  StrToWstr(shader_name),
													  entry_point,
													  target_ver);
		}
		else if (checkTypeSelected<VertexShader>()) {
			(void)CreateShaderFromMemory<VertexShader>(resource_mgr,
													   data,
													   StrToWstr(shader_name),
													   entry_point,
													   target_ver);
		}
	}

private:
	[[nodiscard]]
	bool drawCompilePopup(render::Engine& engine) {
		bool compile_clicked = false;

		if (compile_popup_open) {
			ImGui::OpenPopup("Compile");
			compile_popup_open = false;
		}
		if (overwrite_warning_open) {
			ImGui::OpenPopup("Overwrite Shader");
			overwrite_warning_open = false;
		}

		if (ImGui::BeginPopupModal("Compile", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

			// Shader Name
			ImGui::InputText("ID", &shader_name);
			ImGui::Spacing();

			// Entry Point
			ImGui::InputText("Entry Point", &entry_point);
			ImGui::Spacing();

			// Shader Type
			static const auto type_name_getter = [](void* data, int idx, const char** out_text) {
				if (idx < 0 || idx > std::size(shader_types)) return false;
				*out_text = shader_types[idx].first;
				return true;
			};
			ImGui::Combo("Shader Type", &type_idx, type_name_getter, nullptr, static_cast<int>(std::size(shader_types)));
			ImGui::Spacing();

			// Compile Button
			if (ImGui::Button("Compile")) {
				if (checkExistingShaderName(engine)) {
					overwrite_warning_open = true;
				}
				else {
					compile_clicked = true;
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			// Cancel Button
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Overwrite Shader", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("A shader named \"%s\" already exists. Do you want to overwrite it?", shader_name.c_str());

			if (ImGui::Button("Yes")) {
				compile_clicked = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return compile_clicked;
	}

	[[nodiscard]]
	bool checkExistingShaderName(render::Engine& engine) const {
		using namespace render;

		// Check the selected shader type name, then compare existing names of that type of shader.
		if (checkTypeSelected<ComputeShader>()) {
			return checkTypeExistingName<ComputeShader>(engine);
		}
		else if (checkTypeSelected<DomainShader>()) {
			return checkTypeExistingName<DomainShader>(engine);
		}
		else if (checkTypeSelected<GeometryShader>()) {
			return checkTypeExistingName<GeometryShader>(engine);
		}
		else if (checkTypeSelected<HullShader>()) {
			return checkTypeExistingName<HullShader>(engine);
		}
		else if (checkTypeSelected<PixelShader>()) {
			return checkTypeExistingName<PixelShader>(engine);
		}
		else if (checkTypeSelected<VertexShader>()) {
			return checkTypeExistingName<VertexShader>(engine);
		}

		return false;
	}

	template<typename ShaderT>
	[[nodiscard]]
	bool checkTypeExistingName(render::Engine& engine) const {
		auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
		std::wstring wstr_name = StrToWstr(shader_name);

		for (auto& [guid, shader_ptr] : resource_mgr.getResourceMap<ShaderT>()) {
			if (guid == wstr_name) {
				return true;
			}
		}
		return false;
	}

	template<typename ShaderT>
	[[nodiscard]]
	bool checkTypeSelected() const {
		const char* type_name = shader_types[type_idx].first;

		if constexpr (std::is_same_v<render::ComputeShader, ShaderT>) {
			return (strcmp(type_name, "Compute Shader") == 0);
		}
		else if constexpr (std::is_same_v<render::DomainShader, ShaderT>) {
			return (strcmp(type_name, "Domain Shader") == 0);
		}
		else if constexpr (std::is_same_v<render::GeometryShader, ShaderT>) {
			return (strcmp(type_name, "Geometry Shader") == 0);
		}
		else if constexpr (std::is_same_v<render::HullShader, ShaderT>) {
			return (strcmp(type_name, "Hull Shader") == 0);
		}
		else if constexpr (std::is_same_v<render::PixelShader, ShaderT>) {
			return (strcmp(type_name, "Pixel Shader") == 0);
		}
		else if constexpr (std::is_same_v<render::VertexShader, ShaderT>) {
			return (strcmp(type_name, "Vertex Shader") == 0);
		}

		return false;
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	bool compile_popup_open     = false;
	bool overwrite_warning_open = false;

	std::string shader_name;
	std::string entry_point;

	int type_idx = 0;

	// pair<name, target_version>
	static constexpr std::pair<gsl::czstring, gsl::czstring> shader_types[] = {
	    {"Compute Shader",  "cs_5_0"},
	    {"Domain Shader",   "ds_5_0"},
	    {"Geometry Shader", "gs_5_0"},
	    {"Hull Shader",     "hs_5_0"},
	    {"Pixel Shader",    "ps_5_0"},
	    {"Vertex Shader",   "vs_5_0"},
	};
};