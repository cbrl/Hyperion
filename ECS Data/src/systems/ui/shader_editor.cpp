#include "shader_editor.h"
#include "engine/engine.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_addons/ImGuiColorTextEdit/TextEditor.h"


namespace ShaderEditor {

void CompileShader(Engine& engine, const std::wstring& name, const std::string& shader) {
	ComPtr<ID3DBlob> blob;

	const auto    data   = gsl::make_span<const char>(shader.c_str(), shader.size());
	const HRESULT result = CompileShaderToBytecode(data, "PS", "ps_5_0", gsl::make_not_null(blob.GetAddressOf()));

	if (SUCCEEDED(result)) {
		auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
		(void)resource_mgr.createOrReplace<PixelShader>(name, ShaderBytecodeBlob(blob));
	}
}

void DrawCompilePopup(Engine& engine, TextEditor& editor, bool& compile_popup_open) {
	if (compile_popup_open) {
		ImGui::OpenPopup("Compile");
		compile_popup_open = false;
	}
	if (ImGui::BeginPopupModal("Compile", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		static std::string name;
		ImGui::InputText("ID", &name);
		ImGui::Text("Existing shader will be replaced if the ID is identical");

		ImGui::Spacing();

		if (ImGui::Button("Compile")) {
			CompileShader(engine, StrToWstr(name), editor.GetText());
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}


void OpenFile(TextEditor& editor) {
	const fs::path filename = OpenFileDialog();

	if (not fs::exists(filename))
		return;

	std::ifstream stream(filename);
	if (stream.good()) {
		std::string text{std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{}};
		editor.SetText(text);
	}
}


void SaveFile(const std::string& text) {
	const fs::path filename = SaveFileDialog();

	std::ofstream file(filename, std::ios_base::trunc);
	if (file.good()) {
		file.write(text.c_str(), text.size());
	}
}


void DrawMenuBar(Engine& engine, TextEditor& editor, bool& compile_popup) {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open")) {
				OpenFile(editor);
			}
			if (ImGui::MenuItem("Save")) {
				SaveFile(editor.GetText());
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Compile Shader")) {
				compile_popup = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}


void DrawEditor(Engine& engine, bool& open) {
	static TextEditor editor;
	static bool compile_popup_open = false;

	if (!open) return;

	if (ImGui::Begin("Shader Editor", &open, ImGuiWindowFlags_MenuBar)) {
		DrawMenuBar(engine, editor, compile_popup_open);
		editor.Render("Editor");
	}
	ImGui::End();

	DrawCompilePopup(engine, editor, compile_popup_open);
}

} //namespace ShaderEditor