module;

#include "datatypes/types.h"
#include "io/io.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_addons/ImGuiColorTextEdit/TextEditor.h"

export module systems.user_interface.modules.text_edit_window;

import systems.user_interface.modules.shader_compile_menu;

import rendering;


export class TextEditWindow final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TextEditWindow() {
		newEditor();
	}

	TextEditWindow(const TextEditWindow&) = default;
	TextEditWindow(TextEditWindow&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~TextEditWindow() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	TextEditWindow& operator=(const TextEditWindow&) = default;
	TextEditWindow& operator=(TextEditWindow&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	void draw(Engine& engine, bool& open) {
		if (not open) return;

		if (ImGui::Begin("Text Editor", &open, ImGuiWindowFlags_MenuBar)) {
			drawMenuBar();

			if (ImGui::BeginTabBar("TextEditors", ImGuiTabBarFlags_AutoSelectNewTabs)) {

				// Render each editor tab
				for (size_t i = 0; i < editors.size(); ++i) {
					auto& path   = getPath(i);
					auto& editor = getEditor(i);

					bool open = true;
					if (ImGui::BeginTabItem(path.string().c_str(), &open)) {
						if (ImGui::IsItemClicked()) {
							current = i;
						}
						editor.Render("Editor");
						ImGui::EndTabItem();
					}

					if (not open) {
						closeEditor(i);
					}
				}

				// "New tab" tab
				if (ImGui::BeginTabItem("+", nullptr, ImGuiTabItemFlags_NoPushId | ImGuiTabItemFlags_NoCloseWithMiddleMouseButton)) {
					ImGui::EndTabItem(); //do nothing when this "tab" is opened
				}
				if (ImGui::IsItemClicked()) { //open a new editor when the "+" tab is clicked
					newEditor();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::End();

		if (shader_compile_menu.update(engine)) {
			const std::string text = getCurrEditor().GetText();
			shader_compile_menu.compileShader(engine, text);
		}
	}

private:

	void drawMenuBar() {
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

	void openFile() {
		getCurrPath() = OpenFileDialog();

		if (not fs::exists(getCurrPath()))
			return;

		std::ifstream stream(getCurrPath());
		if (stream.good()) {
			std::string text{ std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{} };
			getCurrEditor().SetText(text);
		}
		else {
			Logger::log(LogLevel::err, "Failed to open \"{}\" for input", getCurrPath().string());
		}
	}

	void saveFile(bool save_as) {
		if (save_as)
			getCurrPath() = SaveFileDialog(); //open the save file dialog every time for the "Save As" option
		else if (not fs::exists(getCurrPath()))
			getCurrPath() = SaveFileDialog(); //else open the save file dialog only if the current path doesn't exist

		std::ofstream file(getCurrPath(), std::ios_base::trunc);
		if (file.good()) {
			const std::string text = getCurrEditor().GetText();
			file.write(text.c_str(), text.size());
		}
		else {
			Logger::log(LogLevel::err, "Failed to open \"{}\" for output", getCurrPath().string());
		}
	}

	void newEditor() {
		auto& editor = editors.emplace_back();
		editor.second = "Untitled-" + ToStr(untitled_n++).value_or(std::string{ "0" });
		current = editors.size() - 1;
	}

	void closeEditor(size_t index) {
		if (index < editors.size()) {
			if (index >= current)
				current--;
			editors.erase(editors.begin() + index);
		}
	}

	TextEditor& getEditor(size_t index) {
		if (index < editors.size())
			return editors[index].first;
		else
			return editors[0].first;
	}

	fs::path& getPath(size_t index) {
		if (index < editors.size())
			return editors[index].second;
		else
			return editors[0].second;
	}
	
	void closeCurrEditor() {
		closeEditor(current);
	}

	TextEditor& getCurrEditor() {
		return getEditor(current);
	}

	fs::path& getCurrPath() {
		return getPath(current);
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	ShaderCompileMenu shader_compile_menu;

	// Vector of text editors and associated file
	std::vector<std::pair<TextEditor, fs::path>> editors;
	size_t current = 0;

	// Used for the initial value of the path of a new editor
	size_t untitled_n = 1;
};