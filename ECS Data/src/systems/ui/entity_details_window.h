#pragma once

#include "new_model_menu.h"
#include "string/string.h"

struct ID3D11Device;

class Engine;
class Input;
class Scene;
class ResourceMgr;
class EntityPtr;

class Transform;


class EntityDetailsWindow final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	EntityDetailsWindow() = default;
	EntityDetailsWindow(const EntityDetailsWindow&) = default;
	EntityDetailsWindow(EntityDetailsWindow&&) noexcept = default;

	
	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~EntityDetailsWindow() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	EntityDetailsWindow& operator=(const EntityDetailsWindow&) = default;
	EntityDetailsWindow& operator=(EntityDetailsWindow&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------

	// Draw the details panel for the specified entity
	void draw(Engine& engine, EntityPtr entity_ptr);

private:

	// Draw the menu in the entity details window
	void drawAddComponentMenu(Engine& engine, EntityPtr entity_ptr);

	// Draw a node in the tree, and its details if selected
	template<typename T, typename... ArgsT>
	void drawComponentNode(gsl::czstring<> text, T& item, ArgsT&&... args);

	template<typename ComponentT, typename... ArgsT>
	void drawDetails(ComponentT& component, ArgsT&&... args);

	template<typename ResourceT>
	void drawResourceMapComboBox(const char* name,
	                             const char* preview_text,
	                             const ResourceMgr& resource_mgr,
	                             std::shared_ptr<ResourceT>& selection_receiver);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	NewModelMenu new_model_menu;

	// "Parent" Combo Box
	int entity_names_idx = 0;
	std::vector<std::reference_wrapper<const std::string>> entity_names_list;

	// Resource Map Combo Box
	std::vector<std::reference_wrapper<const std::string>> res_map_names;

	//ImGuizmo
};