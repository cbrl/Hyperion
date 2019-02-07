#include "user_interface.h"
#include "system_menu.h"
#include "scene_tree.h"
#include "entity_details_window.h"
#include "new_model_menu.h"
#include "selectable_tree.h"
#include "text_edit_window.h"
#include "metrics_window.h"
#include "transform_manipulator.h"

#include "engine/engine.h"

#include "imgui.h"


UserInterface::UserInterface(const Engine& engine) {
	system_menu           = std::make_unique<SystemMenu>(engine);
	scene_tree            = std::make_unique<SceneTree>();
	entity_details        = std::make_unique<EntityDetailsWindow>();
	metrics               = std::make_unique<MetricsWindow>(engine);
	text_editor           = std::make_unique<TextEditWindow>();
	transform_manipulator = std::make_unique<TransformManipulator>();
}


UserInterface::UserInterface(UserInterface&&) noexcept = default;


UserInterface::~UserInterface() = default;


UserInterface& UserInterface::operator=(UserInterface&&) noexcept = default;


void UserInterface::update(Engine& engine) {

	//ImGui::ShowDemoWindow();
	ImGuizmo::BeginFrame();

	auto& device       = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	auto& scene        = engine.getScene();

	// Draw the system menu
	if (engine.getInput().isKeyPressed(Keyboard::F3)) {
		system_menu_open = !system_menu_open;
	}
	if (system_menu_open) {
		system_menu->draw(engine);
	}

	// Draw the system metrics
	metrics->draw(engine);

	// Draw the shader editor
	text_editor->draw(engine, system_menu->getTextEditorState());

	// Draw the scene tree
	scene_tree->draw(scene);

	// Draw the selected entity's details
	entity_details->draw(engine, scene_tree->getSelectedEntity());

	// Draw transform manipulation tool
	transform_manipulator->draw(engine, scene_tree->getSelectedEntity());

	ImGui::End();
}