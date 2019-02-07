#include "user_interface.h"
#include "system_menu.h"
#include "scene_tree.h"
#include "entity_details_window.h"
#include "new_model_menu.h"
#include "selectable_tree.h"
#include "text_edit_window.h"
#include "metrics_window.h"
#include "transform_manipulator.h"

#include "log/log.h"
#include "os/windows/win_utils.h"

#include "engine/engine.h"
#include "resource/resource_mgr.h"
#include "scene/scene.h"

#include "entities/entities.h"
#include "components/components.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_addons/ImGuizmo/ImGuizmo.h"
#include "imgui_addons/metrics_gui/metrics_gui/metrics_gui.h"
#include "imgui_addons/ImGuiColorTextEdit/TextEditor.h"



void UserInterface::update(Engine& engine) {

	//ImGui::ShowDemoWindow();
	ImGuizmo::BeginFrame();

	auto& device       = engine.getRenderingMgr().getDevice();
	auto& resource_mgr = engine.getRenderingMgr().getResourceMgr();
	auto& scene        = engine.getScene();

	// Draw the system menu
	static SystemMenu system_menu(engine);
	static bool system_menu_open = true;
	if (engine.getInput().isKeyPressed(Keyboard::F3)) {
		system_menu_open = !system_menu_open;
	}
	if (system_menu_open) {
		system_menu.draw(engine);
	}

	// Draw the system metrics
	static MetricsWindow metrics(engine);
	metrics.draw(engine);

	// Draw the shader editor
	static TextEditWindow text_editor;
	text_editor.draw(engine, system_menu.getTextEditorState());

	// Draw the scene tree
	static SceneTree scene_tree;
	scene_tree.draw(scene);

	// Draw the selected entity's details
	static EntityDetailsWindow entity_details;
	entity_details.draw(engine, scene_tree.getSelectedEntity());

	// Draw transform manipulation tool
	static TransformManipulator transform_manipulator;
	transform_manipulator.draw(engine, scene_tree.getSelectedEntity());

	ImGui::End();
}