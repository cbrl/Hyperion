#pragma once

#include "system/system.h"
#include "scene/events/core_events.h"
#include "modules/entity_details_window.h"


class Engine;

class SystemMenu;
class SceneTree;
class MetricsWindow;
class TextEditWindow;
class TransformManipulator;
class EntityDetailsWindow;

namespace ecs {
class IComponent;
}


namespace render {

class ResourceMgr;
class Scene;

namespace systems {

class UserInterface final : public ecs::System<UserInterface>, public ecs::EventSender {
public:
	using UserComponent = EntityDetailsWindow::UserComponent;

	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	UserInterface(Engine& engine);
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
	UserInterface& operator=(UserInterface&&) noexcept;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void update() override;

	template<typename ComponentT>
	void registerUserComponent(const UserComponent& component_def) {
		entity_details->registerUserComponent<ComponentT>(component_def);
	}

private:

	using System<UserInterface>::setUpdateInterval; //UI updates every frame

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::reference_wrapper<Engine> engine;

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