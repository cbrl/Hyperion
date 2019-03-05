#include "scene_tree.h"
#include "engine/engine.h"

#include "imgui.h"


void SceneTree::draw(Scene& scene) {
	ImGui::SetNextWindowSize(ImVec2{275, 600}, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar)) {
		drawMenuBar(scene);
		drawTree(scene);
	}
	ImGui::End();
}


void SceneTree::drawTree(Scene& scene) {
	if (ImGui::BeginChild("Object List", {0, 0}, false, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("%s (Entities: %llu)", scene.getName().c_str(), scene.getEntities().size());
		ImGui::Separator();

		// Draw a tree node for each root entity
		auto& entities = scene.getEntities();
		for (const auto& entity_ptr : entities) {
			if (entity_ptr->hasParent()) continue;
			drawEntityNode(entity_ptr);
		}
	}

	ImGui::EndChild();
}


EntityPtr SceneTree::getSelectedEntity() const noexcept {
	return scene_tree.getSelected();
}


void SceneTree::drawEntityNode(EntityPtr entity_ptr) {

	if (!entity_ptr.valid())
		return;

	auto* entity = entity_ptr.get();
	auto  handle = entity_ptr.getHandle();

	//----------------------------------------------------------------------------------
	// Draw Entity Node
	//----------------------------------------------------------------------------------
	bool node_open = true;
	if (entity->hasChildren()) {
		node_open = scene_tree.newNode(entity_ptr, entity->getName().c_str());
	}
	else {
		node_open = scene_tree.newLeafNode(entity_ptr, entity->getName().c_str());
	}

	if (node_open) {
		// Draw any child entities in this node
		if (entity->hasChildren()) {
			entity->forEachChild([this](EntityPtr& child) {
				drawEntityNode(child);
			});
		}

		scene_tree.endNode();
	}
}


void SceneTree::drawMenuBar(Scene& scene) {
	if (ImGui::BeginMenuBar()) {
		drawEntityMenu(scene);
		ImGui::EndMenuBar();
	}
}


void SceneTree::drawEntityMenu(Scene& scene) {

	if (ImGui::BeginMenu("Entity")) {

		if (ImGui::MenuItem("New")) {
			scene.addEntity();
		}

		if (ImGui::BeginMenu("Selected", scene_tree.getSelected().valid())) {
			ImGui::Separator();
			if (ImGui::MenuItem("Delete")) {
				scene.removeEntity(scene_tree.getSelected()->getPtr());
			}

			ImGui::EndMenu(); //Selected
		}

		ImGui::EndMenu(); //Entity
	}
}