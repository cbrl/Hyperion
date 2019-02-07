#pragma once

#include "system/system.h"


class Engine;
class ResourceMgr;
class Scene;

class SystemMenu;
class SceneTree;
class EntityDetailsWindow;
class MetricsWindow;
class TextEditWindow;
class TransformManipulator;


class UserInterface final : public System<UserInterface> {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	UserInterface(const Engine& engine);
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
	void update(Engine& engine) override;

	void setUpdateInterval(f32 seconds) noexcept override {}; //UI updates every frame

private:

	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	std::unique_ptr<SystemMenu>           system_menu;
	std::unique_ptr<SceneTree>            scene_tree;
	std::unique_ptr<EntityDetailsWindow>  entity_details;
	std::unique_ptr<MetricsWindow>        metrics;
	std::unique_ptr<TextEditWindow>       text_editor;
	std::unique_ptr<TransformManipulator> transform_manipulator;

	bool system_menu_open = true;
};