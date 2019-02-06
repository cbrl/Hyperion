#pragma once

#include "datatypes/datatypes.h"

class Engine;

class ShaderCompileMenu {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	ShaderCompileMenu();
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
	void drawMenu();

	// Call every frame to render the compile popup if it is open.
	// Returns true if "Compile" was clicked, and thus compileShader() needs to be called.
	[[nodiscard]]
	bool update(Engine& engine);

	// Compile an input shader of the type selected in this menu
	void compileShader(Engine& engine, gsl::span<const char> data) const;

private:
	[[nodiscard]]
	bool drawCompilePopup(Engine& engine);

	[[nodiscard]]
	bool checkExistingShaderName(Engine& engine) const;

	template<typename ShaderT>
	[[nodiscard]]
	bool checkTypeExistingName(Engine& engine) const;

	template<typename ShaderT>
	[[nodiscard]]
	bool checkTypeSelected() const;


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

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