module;

#include <functional>
#include <memory>

#include "imgui.h"

export module systems.user_interface;

import ecs;

import rendering;

import events.core_events;

import systems.user_interface.modules.entity_details_window;
import systems.user_interface.modules.metrics_window;
import systems.user_interface.modules.new_model_menu;
import systems.user_interface.modules.scene_tree;
import systems.user_interface.modules.system_menu;
import systems.user_interface.modules.text_edit_window;
import systems.user_interface.modules.transform_manipulator;


namespace render {

namespace systems {

export class UserInterface final : public ecs::System {
public:
	using UserComponent = EntityDetailsWindow::UserComponent;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	UserInterface(ecs::ECS& ecs, Engine& engine);
	UserInterface(const UserInterface&) = delete;
	UserInterface(UserInterface&&) noexcept;


	//----------------------------------------------------------------------------------
	// Destructors
	//----------------------------------------------------------------------------------
	~UserInterface();


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	UserInterface& operator=(const UserInterface&) = delete;
	UserInterface& operator=(UserInterface&&) noexcept = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

	template<typename ComponentT>
	void registerUserComponent(const UserComponent& component_def) {
		entity_details->registerUserComponent<ComponentT>(component_def);
	}

private:

	void onEntitySelected(const events::EntitySelectedEvent& event);

	using System<UserInterface>::setUpdateInterval; //UI updates every frame

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<Engine> engine;
	ecs::UniqueDispatcherConnection entity_select_connection;

	std::unique_ptr<SystemMenu>           system_menu;
	std::unique_ptr<SceneTree>            scene_tree;
	std::unique_ptr<EntityDetailsWindow>  entity_details;
	std::unique_ptr<MetricsWindow>        metrics;
	std::unique_ptr<TextEditWindow>       text_editor;
	std::unique_ptr<TransformManipulator> transform_manipulator;

	bool system_menu_open = true;

	bool last_keyboard_state = false;
	bool last_mouse_state    = false;
};

} //namespace systems
} //namespace render
