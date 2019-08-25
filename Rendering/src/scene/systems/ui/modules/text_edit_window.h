#pragma once

#include "datatypes/types.h"
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

	void newEditor();
	void closeEditor(size_t index);

	TextEditor& getEditor(size_t index);
	fs::path& getPath(size_t index);
	
	void closeCurrEditor();
	TextEditor& getCurrEditor();
	fs::path& getCurrPath();


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