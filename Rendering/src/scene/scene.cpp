module;

#include <memory>
#include <string>
#include <utility>

#include "memory/handle/handle.h"

#include "directx/d3d11.h"

module rendering;

import :engine;
import :model_blueprint;
import :components.hierarchy;
import :components.transform;
import :systems.camera_system;
import :systems.hierarchy_system;
import :systems.model_system;
import :systems.transform_system;


namespace render {

void Scene::load(Engine& engine) {
	addCoreSystems(engine);
	initialize(engine);
}

void Scene::tick(Engine& engine) {
	ecs.update(engine.getTimer().deltaTime());
	this->update(engine);
}

handle64 Scene::importModel(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint) {
	auto handle = createEntity();
	importModel(handle, device, blueprint);
	return handle;
}

void Scene::importModel(handle64 handle, ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint) {
	if (not ecs.valid(handle))
		return;

	if (not ecs.has<Hierarchy>(handle))
		ecs.add<Hierarchy>(handle);

	std::function<void(handle64, const std::shared_ptr<ModelBlueprint>&, const ModelBlueprint::Node&)> process_node =
		[&](handle64 handle, const std::shared_ptr<ModelBlueprint>& bp, const ModelBlueprint::Node& bp_node) {
			// Construct each model at this node
			for (const u32 index : bp_node.mesh_indices) {
				auto child = createEntity<EntityTemplates::HierarchyT>();
				ecs.get<Hierarchy>(handle).addChild(ecs, child);
				ecs.add<Model>(child, device, bp, index);
			}

			// Add a child entity for each child node
			for (auto& node : bp_node.child_nodes) {
				auto child_root = createEntity();
				ecs.get<Hierarchy>(handle).addChild(ecs, child_root);
				process_node(child_root, bp, node);
			}
		};

	process_node(handle, blueprint, blueprint->root);
}

void Scene::addCoreSystems(const Engine& engine) {
	// Transform system: updates transform components when they're modified
	ecs.add<systems::TransformSystem>();

	// Hierarchy system: ensures the hierarchy is properly deleted when destroying an entity
	ecs.add<systems::HierarchySystem>();

	// Camera system: updates the buffers of camera components
	ecs.add<systems::CameraSystem>(engine.getRenderingMgr());

	// Model system: updates the buffers of model components
	ecs.add<systems::ModelSystem>(engine.getRenderingMgr());
}

} //namespace render
