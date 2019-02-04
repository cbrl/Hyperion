#include "text_edit_window.h"
#include "engine/engine.h"

#include "misc/cpp/imgui_stdlib.h"


void TextEditWindow::drawEditor(Engine& engine, bool& open) {
	if (!open) return;

	if (ImGui::Begin("Text Editor", &open, ImGuiWindowFlags_MenuBar)) {
		drawMenuBar();
		editor.Render("Editor");
	}
	ImGui::End();

	if (shader_compile_menu.update(engine)) {
		std::string text = editor.GetText();
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
	current_file = OpenFileDialog();

	if (not fs::exists(current_file))
		return;

	std::ifstream stream(current_file);
	if (stream.good()) {
		std::string text{std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{}};
		editor.SetText(text);
	}
}


void TextEditWindow::saveFile(bool save_as) {
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
