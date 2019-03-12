#pragma once

#include "system/system.h"
#include "scene/events/core_events.h"
#include "modules/entity_details_window.h"


class Engine;
class ResourceMgr;
class Scene;
class IComponent;

class SystemMenu;
class SceneTree;
class MetricsWindow;
class TextEditWindow;
class TransformManipulator;


class UserInterface final : public System<UserInterface>, public EventSender {
public:
	using UserComponent = EntityDetailsWindow::UserComponent;

public:
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

	void setUpdateInterval(f32 seconds) noexcept override {}; //UI updates every frame

	template<typename ComponentT>
	void registerUserComponent(const UserComponent& component_def) {
		entity_details->registerUserComponent<ComponentT>(component_def);
	}

private:

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