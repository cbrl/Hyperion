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


void Scene::removeSystem(ISystem* system) {
	ecs->removeSystem(system);
}


void Scene::tick(Engine& engine) {
	ecs->update(engine, engine.getTimer().deltaTime());
	this->update(engine);
}