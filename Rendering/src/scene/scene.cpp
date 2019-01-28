#include "scene.h"
#include "engine/engine.h"
#include "scene/systems/core_systems.h"


void Scene::load(const Engine& engine) {
	addCoreSystems(engine);
	initialize(engine);
}


void Scene::addCoreSystems(const Engine& engine) {

	// Transform system: updates transform components when they're modified
	ecs->addSystem<TransformSystem>();

	// Camera system: updates the buffers of camera components
	ecs->addSystem<CameraSystem>();

	// Model system: updates the buffers of model components
	ecs->addSystem<ModelSystem>();
}


void Scene::removeEntity(EntityPtr entity) {
	if (entity) {
		ecs->destroyEntity(entity.getHandle());

		if (auto parent = entity->getParent()) {
			parent->removeChild(entity);
		}
	}
	entities.erase(std::remove(std::begin(entities), std::end(entities), entity),
	               std::end(entities));
}


void Scene::removeSystem(ISystem& system) {
	ecs->removeSystem(system);
}


void Scene::tick(Engine& engine) {
	ecs->update(engine, static_cast<f32>(engine.getTimer().deltaTime()));
	this->update(engine);
}


EntityPtr Scene::importModel(ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint) {
	EntityPtr ptr = addEntity();
	importModel(ptr, device, blueprint);
	return ptr;
}


void Scene::importModel(const EntityPtr& ptr, ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& blueprint) {

	std::function<void(Entity&, const std::shared_ptr<ModelBlueprint>&, const ModelBlueprint::Node&)> process_node =
		[&](Entity& entity, const std::shared_ptr<ModelBlueprint>& bp, const ModelBlueprint::Node& bp_node) {
			// Construct each model at this node
			for (const u32 index : bp_node.mesh_indices) {
			    entity.addComponent<Model>(device, bp, index);
			}

			// Add a child entity for each child node
			for (auto& node : bp_node.child_nodes) {
			    auto child = addEntity();
			    entity.addChild(child);
				process_node(*child, bp, node);
			}
		};

	process_node(*ptr, blueprint, blueprint->root);
}