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


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	// Text editor visibility
	bool text_editor = false;

	// Display modes
	int current_display_desc = 0;
	std::vector<std::string> display_modes;

	// Shadow map resolution
	u32 smap_res = 0;
};