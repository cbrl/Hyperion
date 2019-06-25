#include "scene.h"
#include "engine/engine.h"
#include "scene/systems/core_systems.h"


namespace render {

void Scene::load(Engine& engine) {
	addCoreSystems(engine);
	initialize(engine);
}


void Scene::addCoreSystems(const Engine& engine) {

	// Transform system: updates transform components when they're modified
	ecs.addSystem<systems::TransformSystem>();

	// Camera system: updates the buffers of camera components
	ecs.addSystem<systems::CameraSystem>(engine.getRenderingMgr());

	// Model system: updates the buffers of model components
	ecs.addSystem<systems::ModelSystem>(engine.getRenderingMgr());
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

	std::function<void(handle64, const std::shared_ptr<ModelBlueprint>&, const ModelBlueprint::Node&)> process_node =
	    [&](handle64 handle, const std::shared_ptr<ModelBlueprint>& bp, const ModelBlueprint::Node& bp_node) {
			// Construct each model at this node
			for (const u32 index : bp_node.mesh_indices) {
				auto child = createEntity();
				ecs.get(handle).addChild(child);
				ecs.get(handle).addComponent<Model>(device, bp, index);
			}

			// Add a child entity for each child node
			for (auto& node : bp_node.child_nodes) {
			    auto child_root = createEntity();
				ecs.get(handle).addChild(child_root);
				process_node(child_root, bp, node);
			}
		};

	process_node(handle, blueprint, blueprint->root);
}

} //namespace render