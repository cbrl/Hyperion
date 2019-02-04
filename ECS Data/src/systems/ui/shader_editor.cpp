#include "shader_editor.h"
#include "engine/engine.h"

#include "misc/cpp/imgui_stdlib.h"


ShaderEditor::ShaderEditor()
	: compile_popup_open(false)
	, overwrite_warning_open(false)
	, type_idx(0) {
}


void ShaderEditor::drawEditor(Engine& engine, bool& open) {
	if (!open) return;

	if (ImGui::Begin("Shader Editor", &open, ImGuiWindowFlags_MenuBar)) {
		drawMenuBar();
		editor.Render("Editor");
	}
	ImGui::End();

	drawCompilePopup(engine);
}


void ShaderEditor::drawMenuBar() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open...")) {
				openFile();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save")) {
				saveFile(false);
			}
			if (ImGui::MenuItem("Save As...")) {
				saveFile(true);
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Compile Shader")) {
				compile_popup_open = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}


void ShaderEditor::openFile() {
	current_file = OpenFileDialog();

	if (not fs::exists(current_file))
		return;

	std::ifstream stream(current_file);
	if (stream.good()) {
		std::string text{std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{}};
		editor.SetText(text);
	}
}


void ShaderEditor::saveFile(bool save_as) {
	if (save_as)
		current_file = SaveFileDialog();  //open the save file dialog every time for the "Save As" option
	else if (not fs::exists(current_file))
		current_file = SaveFileDialog();  //else open the save file dialog only if the current path doesn't exist

	std::ofstream file(current_file, std::ios_base::trunc);
	if (file.good()) {
		const std::string text = editor.GetText();
		file.write(text.c_str(), text.size());
	}
}


void ShaderEditor::drawCompilePopup(Engine& engine) {
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
				compileShader(engine);
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
			compileShader(engine);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}


void ShaderEditor::compileShader(Engine& engine) const {
	ComPtr<ID3DBlob> blob;

	const std::string shader = editor.GetText();
	const auto        data   = gsl::make_span<const char>(shader.c_str(), shader.size());
	const HRESULT     result = CompileShaderToBytecode(data,
	                                                   entry_point.c_str(),
	                                                   shader_types[type_idx].second,
	                                                   gsl::make_not_null(blob.GetAddressOf()));

	if (SUCCEEDED(result)) {
		auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
		(void)resource_mgr.createOrReplace<PixelShader>(StrToWstr(shader_name), ShaderBytecodeBlob(blob));
	}
}


bool ShaderEditor::checkExistingShaderName(Engine& engine) const {
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
bool ShaderEditor::checkTypeNames(Engine& engine) const {
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	std::wstring wstr_name = StrToWstr(shader_name);

	for (auto& [guid, shader_ptr] : resource_mgr.getResourceMap<ShaderT>()) {
		if (guid == wstr_name) {
			return true;
		}
	}
	return false;
}