#include "shader_compile_menu.h"
#include "engine/engine.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"


ShaderCompileMenu::ShaderCompileMenu()
	: compile_popup_open(false)
	, overwrite_warning_open(false)
	, type_idx(0) {
}


void ShaderCompileMenu::drawMenu() {
	if (ImGui::BeginMenu("Shader")) {
		if (ImGui::MenuItem("Compile Shader")) {
			compile_popup_open = true;
		}
		ImGui::EndMenu();
	}
}


bool ShaderCompileMenu::update(Engine& engine) {
	return drawCompilePopup(engine);
}


bool ShaderCompileMenu::drawCompilePopup(Engine& engine) {

	bool out_compile = false;

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
			if (checkExistingShaderName(engine))
				overwrite_warning_open = true;
			else
				out_compile = true;
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
			out_compile = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No")) {
			out_compile = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	return out_compile;
}


void ShaderCompileMenu::compileShader(Engine& engine, gsl::span<const char> data) const {

	ComPtr<ID3DBlob> blob;
	const HRESULT result = CompileShaderToBytecode(data,
	                                               entry_point.c_str(),
	                                               shader_types[type_idx].second,
	                                               gsl::make_not_null(blob.GetAddressOf()));

	if (SUCCEEDED(result)) {
		auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
		(void)resource_mgr.createOrReplace<PixelShader>(StrToWstr(shader_name), ShaderBytecodeBlob(blob));
	}
}


bool ShaderCompileMenu::checkExistingShaderName(Engine& engine) const {
	const char* type_name = shader_types[type_idx].first;

	// Check the selected shader type name, then compare existing names of that type of shader.
	if (strcmp(type_name, "Compute Shader") == 0) {
		return checkTypeNames<ComputeShader>(engine);
	}
	else if (strcmp(type_name, "Domain Shader") == 0) {
		return checkTypeNames<DomainShader>(engine);
	}
	else if (strcmp(type_name, "Geometry Shader") == 0) {
		return checkTypeNames<GeometryShader>(engine);
	}
	else if (strcmp(type_name, "Hull Shader") == 0) {
		return checkTypeNames<HullShader>(engine);
	}
	else if (strcmp(type_name, "Pixel Shader") == 0) {
		return checkTypeNames<PixelShader>(engine);
	}
	else if (strcmp(type_name, "Vertex Shader") == 0) {
		return checkTypeNames<VertexShader>(engine);
	}

	return false;
}


template<typename ShaderT>
bool ShaderCompileMenu::checkTypeNames(Engine& engine) const {
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	std::wstring wstr_name = StrToWstr(shader_name);

	for (auto& [guid, shader_ptr] : resource_mgr.getResourceMap<ShaderT>()) {
		if (guid == wstr_name) {
			return true;
		}
	}
	return false;
}