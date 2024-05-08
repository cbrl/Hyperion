module;

#include <string>

#include "imgui.h"
#include "memory/handle/handle.h"
#include "string/string.h"

export module rendering:systems.user_interface.modules.scene_tree;

import ecs;
import :systems.user_interface.modules.selectable_tree;
import :components.hierarchy;
import :components.name;
import :scene;


export class SceneTree final {
public:
	//----------------------------------------------------------------------------------
	// Constructors
	//----------------------------------------------------------------------------------
	SceneTree() = default;
	SceneTree(const SceneTree&) = default;
	SceneTree(SceneTree&&) = default;


	//----------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------
	~SceneTree() = default;


	//----------------------------------------------------------------------------------
	// Operators
	//----------------------------------------------------------------------------------
	SceneTree& operator=(const SceneTree&) = default;
	SceneTree& operator=(SceneTree&&) = default;


	//----------------------------------------------------------------------------------
	// Member Functions
	//----------------------------------------------------------------------------------
	void draw(render::Scene& scene) {
		ImGui::SetNextWindowSize(ImVec2{ 275, 600 }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar)) {
			drawMenuBar(scene);
			drawTree(scene);
		}
		ImGui::End();
	}

	[[nodiscard]]
	handle64 getSelectedEntity() const noexcept {
		return static_cast<handle64>(scene_tree.getSelected());
	}

	[[nodiscard]]
	void setSelectedEntity(handle64 entity) noexcept {
		scene_tree.setSelected(entity);
	}

private:

	void drawTree(render::Scene& scene) {
		auto& ecs = scene.getECS();

		if (ImGui::BeginChild("Object List", {0, 0}, false, ImGuiWindowFlags_AlwaysAutoResize)) {

			ImGui::Text("%s (Entities: %llu)", scene.getName().c_str(), scene.getECS().count<handle64>());
			ImGui::Separator();

			// Draw a tree node for each root entity
			ecs.forEach([&](handle64 entity) {
				if (auto* hierarchy = ecs.tryGet<Hierarchy>(entity)) {
					if (ecs.valid(hierarchy->getParent()))
						return;
				}
				drawEntityNode(ecs, entity);
			});
		}

		ImGui::EndChild();
	}

	void drawEntityNode(ecs::ECS& ecs, handle64 entity) {
		bool node_open = true;
		auto* hierarchy = ecs.tryGet<Hierarchy>(entity);

		if (hierarchy && hierarchy->hasChildren()) {
			if (auto* name_cmp = ecs.tryGet<Name>(entity)) {
				node_open = scene_tree.newNode(entity, name_cmp->name.c_str());
			}
			else {
				const std::string name = "(Index: " + ToStr(entity.index).value_or("-1"s) +
										 ", Counter: " + ToStr(entity.counter).value_or("-1"s) + ")";
				node_open = scene_tree.newNode(entity, name.c_str());
			}
		}
		else {
			if (auto* name_cmp = ecs.tryGet<Name>(entity)) {
				node_open = scene_tree.newLeafNode(entity, name_cmp->name.c_str());
			}
			else {
				const std::string name = "(Index: " + ToStr(entity.index).value_or("-1"s) +
										 ", Counter: " + ToStr(entity.counter).value_or("-1"s) + ")";
				node_open = scene_tree.newLeafNode(entity, name.c_str());
			}
		}

		if (node_open) {
			// Draw any child entities in this node
			if (hierarchy && hierarchy->hasChildren()) {
				hierarchy->forEachChild(ecs, [this, &ecs](handle64 child) {
					drawEntityNode(ecs, child);
				});
			}

			scene_tree.endNode();
		}
	}

	void drawMenuBar(render::Scene& scene) {
		if (ImGui::BeginMenuBar()) {
			drawEntityMenu(scene);
			ImGui::EndMenuBar();
		}
	}

	void drawEntityMenu(render::Scene& scene) {
		auto& ecs = scene.getECS();
		const auto handle = static_cast<handle64>(scene_tree.getSelected());

		if (ImGui::BeginMenu("Entity")) {

			if (ImGui::MenuItem("New")) {
				scene.createEntity();
			}

			if ( ImGui::BeginMenu("Selected", ecs.valid(handle)) ) {
				ImGui::Separator();
				if (ImGui::MenuItem("Delete")) {
					ecs.destroy(handle);
				}

				ImGui::EndMenu(); //Selected
			}

			ImGui::EndMenu(); //Entity
		}
	}


	//----------------------------------------------------------------------------------
	// Member Variables
	//----------------------------------------------------------------------------------
	SelectableTree<handle64::value_type> scene_tree;
};