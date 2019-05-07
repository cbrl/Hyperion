#pragma once

#include "datatypes/datatypes.h"

class Engine;

class SystemMenu final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SystemMenu(const Engine& engine);
	SystemMenu(const SystemMenu&) = default;
	SystemMenu(SystemMenu&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SystemMenu() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SystemMenu& operator=(const SystemMenu&) = default;
	SystemMenu& operator=(SystemMenu&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void draw(Engine& engine);

	[[nodiscard]]
	bool& getTextEditorState() noexcept;

private:

	void drawEngineSettingsPopup(Engine& engine);
	void drawDisplaySettingsPopup(Engine& engine);
	void drawExportModelPopup(Engine& engine);
	void drawExportTexturePopup(Engine& engine);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Text editor visibility
	bool text_editor = false;

	// Display modes
	int current_display_desc = 0;
	std::vector<std::string> display_modes;

	// Shadow map variables
	u32 smap_res = 0;
	i32 smap_depth_bias = 0;
	f32 smap_slope_scaled_depth_bias = 0;
	f32 smap_depth_bias_clamp = 0;
};