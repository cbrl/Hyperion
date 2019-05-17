#include "scene_tree.h"
#include "engine/engine.h"

#include "imgui.h"


void SceneTree::draw(render::Scene& scene) {
	ImGui::SetNextWindowSize(ImVec2{275, 600}, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar)) {
		drawMenuBar(scene);
		drawTree(scene);
	}
	ImGui::End();
}


void SceneTree::drawTree(render::Scene& scene) {
	if (ImGui::BeginChild("Object List", {0, 0}, false, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("%s (Entities: %llu)", scene.getName().c_str(), scene.countOf<ecs::Entity>());
		ImGui::Separator();

		// Draw a tree node for each root entity
		scene.forEach([&](const ecs::Entity& entity) {
			if (entity.hasParent()) return;
			drawEntityNode(entity);
		});
	}

	ImGui::EndChild();
}


ecs::EntityPtr SceneTree::getSelectedEntity() const noexcept {
	return scene_tree.getSelected();
}


void SceneTree::setSelectedEntity(const ecs::EntityPtr& entity) noexcept {
	scene_tree.setSelected(entity);
}


void SceneTree::drawEntityNode(const ecs::Entity& entity) {

	auto handle = entity.getPtr().getHandle();

	//----------------------------------------------------------------------------------
	// Draw Entity Node
	//----------------------------------------------------------------------------------
	bool node_open = true;
	if (entity.hasChildren()) {
		node_open = scene_tree.newNode(entity.getPtr(), entity.getName().c_str());
	}
	else {
		node_open = scene_tree.newLeafNode(entity.getPtr(), entity.getName().c_str());
	}

	if (node_open) {
		// Draw any child entities in this node
		if (entity.hasChildren()) {
			entity.forEachChild([this](const ecs::Entity& child) {
				drawEntityNode(child);
			});
		}

		scene_tree.endNode();
	}
}


void SceneTree::drawMenuBar(render::Scene& scene) {
	if (ImGui::BeginMenuBar()) {
		drawEntityMenu(scene);
		ImGui::EndMenuBar();
	}
}


void SceneTree::drawEntityMenu(render::Scene& scene) {

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