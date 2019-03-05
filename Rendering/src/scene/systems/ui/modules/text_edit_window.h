#pragma once

#include "datatypes/datatypes.h"
#include "io/io.h"

#include "shader_compile_menu.h"

#include "imgui.h"
#include "imgui_addons/ImGuiColorTextEdit/TextEditor.h"

class Engine;

class TextEditWindow final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	TextEditWindow();
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
	void draw(Engine& engine, bool& open);

private:
	void drawMenuBar();

	void openFile();
	void saveFile(bool save_as);

	std::pair<TextEditor, fs::path>& newEditor();

	TextEditor& getCurrEditor();
	fs::path& getCurrPath();


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::vector<std::pair<TextEditor, fs::path>> editors;
	std::reference_wrapper<std::pair<TextEditor, fs::path>> current_editor;
	//TextEditor editor;
	//fs::path current_file;

	ShaderCompileMenu shader_compile_menu;
};