#include "text_edit_window.h"
#include "engine/engine.h"

#include "misc/cpp/imgui_stdlib.h"


TextEditWindow::TextEditWindow()
	: current_editor(newEditor()) {

}


std::pair<TextEditor, fs::path>& TextEditWindow::newEditor() {
	static size_t n = 1;
	auto& editor = editors.emplace_back();
	editor.second = "Untitled-" + ToStr(n++).value_or(std::string{"0"});
	return editor;
}


TextEditor& TextEditWindow::getCurrEditor() {
	return current_editor.get().first;
}


fs::path& TextEditWindow::getCurrPath() {
	return current_editor.get().second;
}


void TextEditWindow::draw(Engine& engine, bool& open) {
	if (!open) return;

	if (ImGui::Begin("Text Editor", &open, ImGuiWindowFlags_MenuBar)) {
		drawMenuBar();

		if (ImGui::BeginTabBar("TextEditors")) {
			for (auto& editor : editors) {
				if (ImGui::BeginTabItem(editor.second.empty() ? "New File" : editor.second.string().c_str())) {
					if (ImGui::IsItemClicked()) {
						current_editor = editor;
					}
					ImGui::EndTabItem();
				}
			}

			if (ImGui::BeginTabItem("+", nullptr, ImGuiTabItemFlags_NoPushId)) {
				ImGui::EndTabItem();
			}
			if (ImGui::IsItemClicked()) {
				current_editor = newEditor();
			}

			ImGui::EndTabBar();
		}

		getCurrEditor().Render("Editor");
	}
	ImGui::End();

	if (shader_compile_menu.update(engine)) {
		const std::string text = getCurrEditor().GetText();
		shader_compile_menu.compileShader(engine, gsl::make_span(text));
	}
}


void TextEditWindow::drawMenuBar() {
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

			ImGui::EndMenu();
		}

		shader_compile_menu.drawMenu();

		ImGui::EndMenuBar();
	}
}


void TextEditWindow::openFile() {
	getCurrPath() = OpenFileDialog();

	if (not fs::exists(getCurrPath()))
		return;

	std::ifstream stream(getCurrPath());
	if (stream.good()) {
		std::string text{std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{}};
		getCurrEditor().SetText(text);
	}
}


void TextEditWindow::saveFile(bool save_as) {
	if (save_as)
		getCurrPath() = SaveFileDialog(); //open the save file dialog every time for the "Save As" option
	else if (not fs::exists(getCurrPath()))
		getCurrPath() = SaveFileDialog(); //else open the save file dialog only if the current path doesn't exist

	std::ofstream file(getCurrPath(), std::ios_base::trunc);
	if (file.good()) {
		const std::string text = getCurrEditor().GetText();
		file.write(text.c_str(), text.size());
	}
}
