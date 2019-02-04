#pragma once

#include "datatypes/datatypes.h"
#include "io/io.h"

#include "imgui.h"
#include "imgui_addons/ImGuiColorTextEdit/TextEditor.h"

class Engine;

class ShaderEditor {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShaderEditor();
	ShaderEditor(const ShaderEditor&) = default;
	ShaderEditor(ShaderEditor&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~ShaderEditor() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	ShaderEditor& operator=(const ShaderEditor&) = default;
	ShaderEditor& operator=(ShaderEditor&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void drawEditor(Engine& engine, bool& open);

private:

	void drawMenuBar();
	void drawCompilePopup(Engine& engine);

	void openFile();
	void saveFile(bool save_as);
	
	void compileShader(Engine& engine) const;
	bool checkExistingShaderName(Engine& engine) const;

	template<typename ShaderT>
	bool checkTypeNames(Engine& engine) const;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	TextEditor editor;
	fs::path current_file;

	// Compile menu variables
	bool compile_popup_open;
	bool overwrite_warning_open;
	std::string shader_name;
	std::string entry_point;
	int type_idx;

	// pair<name, target_version>
	static constexpr std::pair<gsl::czstring<>, gsl::czstring<>> shader_types[] = {
	    {"Compute Shader",  "cs_5_0"},
	    {"Domain Shader",   "ds_5_0"},
	    {"Geometry Shader", "gs_5_0"},
	    {"Hull Shader",     "hs_5_0"},
	    {"Pixel Shader",    "ps_5_0"},
	    {"Vertex Shader",   "vs_5_0"},
	};
};