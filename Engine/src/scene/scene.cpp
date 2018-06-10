#include "stdafx.h"
#include "scene.h"
#include "engine/engine.h"


void Scene::unload(const Engine& engine) {
	auto& ecs = engine.getECS();

	for (auto& handle : entities) {
		ecs.destroyEntity(handle);
	}
}