#pragma once

#include "new_model_menu.h"
#include "string/string.h"

struct ID3D11Device;

class Engine;
class Input;
class Scene;
class ResourceMgr;
class Texture;
class IComponent;
class EntityPtr;

class Transform;


class EntityDetailsWindow final {
	using ComponentDetailsFunc = std::function<void(IComponent&)>;

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

	template<typename ComponentT>
	void addComponentDetailsRenderer(gsl::czstring<> name, const std::function<void(IComponent&)>& renderer) {
		static_assert(std::is_base_of_v<IComponent, ComponentT>,
		              "EntityDetailsWindow::addComponentDetailsRenderer() - Invalid component type specified");

		component_renderers[ComponentT::index] = std::make_pair(name, renderer);
	}

private:

	// Draw the menu in the entity details window
	void drawAddComponentMenu(Engine& engine, EntityPtr entity_ptr);

	// Draw a node in the tree, and its details if selected
	template<typename T, typename... ArgsT>
	void drawComponentNode(gsl::czstring<> text, T& component, ArgsT&&... args);

	void drawUserComponentNode(gsl::czstring<> text, IComponent& component, const ComponentDetailsFunc& draw_func);

	template<typename ComponentT, typename... ArgsT>
	void drawDetails(ComponentT& component, ArgsT&&... args);

	template<typename ResourceT>
	void drawResourceMapComboBox(const char* name,
	                             const char* preview_text,
	                             ResourceMgr& resource_mgr,
	                             std::shared_ptr<ResourceT>& selection_receiver);

	template<>
	void drawResourceMapComboBox(const char* name,
	                             const char* preview_text,
	                             ResourceMgr& resource_mgr,
	                             std::shared_ptr<Texture>& selection_receiver);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	NewModelMenu new_model_menu;

	// Variables used for popups when creating a new texture
	f32_4       new_color_texture_color;
	std::string new_file_texture_path;

	// "Parent" Combo Box
	int entity_names_idx = 0;
	std::vector<std::reference_wrapper<const std::string>> entity_names_list;

	// Resource Map Combo Box
	std::vector<std::reference_wrapper<const std::string>> res_map_names;

	// Functions to draw the details of user-added components
	std::unordered_map< std::type_index, std::pair<gsl::czstring<>, ComponentDetailsFunc> > component_renderers;
};