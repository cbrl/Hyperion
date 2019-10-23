#pragma once

#include "new_model_menu.h"
#include "string/string.h"
#include "memory/handle/handle.h"

struct ID3D11Device;
class Engine;
class Transform;

namespace render {
class Scene;
class ResourceMgr;
class Texture;
}

namespace ecs {
class ECS;
class IComponent;
}


class EntityDetailsWindow final {
public:
	//----------------------------------------------------------------------------------
	// UserComponent
	//----------------------------------------------------------------------------------
	struct UserComponent final {
		using adder_func = std::function<void(ecs::ECS&, handle64)>;
		using getter_func = std::function<ecs::IComponent*(ecs::ECS&, handle64)>;
		using details_func = std::function<void(ecs::IComponent&)>;

		// The display name of this component
		std::string name;

		// A function that attempts to retrieve the component from an entity
		getter_func getter;

		// A function that handles adding the component to a given entity
		adder_func adder;

		// A function that draws the component's details in ImGui
		details_func details_renderer;
	};


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
	void draw(Engine& engine, handle64 handle);

	template<typename ComponentT>
	requires std::derived_from<ComponentT, ecs::IComponent>
	void registerUserComponent(const UserComponent& component_def) {
		user_components[ComponentT::index] = component_def;
	}

private:

	// Draw the menu in the entity details window
	void drawAddComponentMenu(Engine& engine, ecs::ECS& ecs, handle64 handle);

	// Draw a node in the tree, and its details if selected
	template<typename T, typename... ArgsT>
	void drawComponentNode(ecs::ECS& ecs, gsl::czstring<> text, T& component, ArgsT&&... args);

	void drawUserComponentNode(ecs::ECS& ecs, gsl::czstring<> text, ecs::IComponent& component, const UserComponent::details_func& draw_func);

	template<typename ComponentT, typename... ArgsT>
	void drawDetails(ComponentT& component, ArgsT&&... args);

	template<typename ResourceT>
	void drawResourceMapComboBox(const char* name,
	                             const char* preview_text,
	                             render::ResourceMgr& resource_mgr,
	                             std::shared_ptr<ResourceT>& selection_receiver);

	template<>
	void drawResourceMapComboBox(const char* name,
	                             const char* preview_text,
	                             render::ResourceMgr& resource_mgr,
	                             std::shared_ptr<render::Texture>& selection_receiver);


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------

	NewModelMenu new_model_menu;

	// Variables used for popups when creating a new texture
	f32_4       new_color_texture_color;
	std::string new_file_texture_path;

	// "Parent" Combo Box
	int entity_names_idx = 0;
	std::vector<handle64> entity_list;

	// Resource Map Combo Box
	std::vector<std::reference_wrapper<const std::string>> res_map_names;

	// User component definitions. Each component definiton contains functions that handle
	// adding a new component of that type, and drawing its details in ImGui.
	std::unordered_map<std::type_index, UserComponent> user_components;
};