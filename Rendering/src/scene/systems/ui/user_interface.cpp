#include "user_interface.h"
#include "modules/system_menu.h"
#include "modules/scene_tree.h"
#include "modules/new_model_menu.h"
#include "modules/selectable_tree.h"
#include "modules/text_edit_window.h"
#include "modules/metrics_window.h"
#include "modules/transform_manipulator.h"

#include "engine/engine.h"

#include "imgui.h"


namespace render::systems {

UserInterface::UserInterface(ecs::ECS& ecs, Engine& engine)
	: System(ecs)
	, engine(engine)
	, entity_select_connection(ecs.getDispatcher<events::EntitySelectedEvent>().addCallback<&UserInterface::onEntitySelected>(this)) {
	system_menu           = std::make_unique<SystemMenu>(engine);
	scene_tree            = std::make_unique<SceneTree>();
	entity_details        = std::make_unique<EntityDetailsWindow>();
	metrics               = std::make_unique<MetricsWindow>(engine);
	text_editor           = std::make_unique<TextEditWindow>();
	transform_manipulator = std::make_unique<TransformManipulator>(engine.getInput(), engine.getKeyConfig());

	ImGui::StyleColorsDark();

	// Bind system menu key if not bound
	if (engine.getKeyConfig().bindIfNotBound("SystemMenu", Keyboard::F3)) {
		engine.saveConfig();
	}
}


UserInterface::UserInterface(UserInterface&&) noexcept = default;


UserInterface::~UserInterface() = default;


UserInterface& UserInterface::operator=(UserInterface&&) noexcept = default;


void UserInterface::onEntitySelected(const events::EntitySelectedEvent& event) {
	scene_tree->setSelectedEntity(event.entity);
}


void UserInterface::update() {

	//ImGui::ShowDemoWindow();
	ImGuizmo::BeginFrame();

	auto& input        = engine.get().getInput();
	auto& key_config   = engine.get().getKeyConfig();
	auto& scene        = engine.get().getScene();
	auto& device       = engine.get().getRenderingMgr().getDevice();
	auto& resource_mgr = engine.get().getRenderingMgr().getResourceMgr();

	// Draw the system menu
	if (input.isKeyPressed(key_config.getKey("SystemMenu"))) {
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

	// Send a gui focus event when the focus state changes
	const auto& io = ImGui::GetIO();
	const bool keyboard_state = io.WantCaptureKeyboard;
	const bool mouse_state    = io.WantCaptureMouse;

	if (keyboard_state != last_keyboard_state || mouse_state != last_mouse_state) {
		last_keyboard_state = keyboard_state;
		last_mouse_state    = mouse_state;
		getECS().enqueue<events::GuiFocusEvent>(keyboard_state, mouse_state);
	}
}

} //namespace render