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
	ecs->addSystem<systems::TransformSystem>();

	// Camera system: updates the buffers of camera components
	ecs->addSystem<systems::CameraSystem>(engine.getRenderingMgr());

	// Model system: updates the buffers of model components
	ecs->addSystem<systems::ModelSystem>(engine.getRenderingMgr());
}


void Scene::removeEntity(ecs::EntityPtr entity) {
	if (entity) {
		ecs->destroyEntity(entity.getHandle());
	}
}


void Scene::removeEntity(ecs::Entity& entity) {
	ecs->destroyEntity(entity.getPtr().getHandle());
}


void Scene::removeSystem(ecs::ISystem& system) {
	ecs->removeSystem(system);
}


void Scene::tick(Engine& engine) {
	ecs->update(engine.getTimer().deltaTime());
	this->update(engine);
}


ecs::EntityPtr Scene::importModel(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint) {
	ecs::EntityPtr ptr = addEntity();
	importModel(ptr, device, blueprint);
	return ptr;
}


void Scene::importModel(const ecs::EntityPtr& ptr, ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint) {

	std::function<void(ecs::EntityPtr, const std::shared_ptr<ModelBlueprint>&, const ModelBlueprint::Node&)> process_node =
	    [&](ecs::EntityPtr entity, const std::shared_ptr<ModelBlueprint>& bp, const ModelBlueprint::Node& bp_node) {
			// Construct each model at this node
			for (const u32 index : bp_node.mesh_indices) {
				auto child = addEntity();
				entity->addChild(child);
			    child->addComponent<Model>(device, bp, index);
			}

			// Add a child entity for each child node
			for (auto& node : bp_node.child_nodes) {
			    auto child_root = addEntity();
			    entity->addChild(child_root);
				process_node(child_root, bp, node);
			}
		};

	process_node(ptr, blueprint, blueprint->root);
}

} //namespace render