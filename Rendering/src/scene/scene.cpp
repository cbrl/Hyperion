#include "scene.h"


void Scene::onResize(vec2_u32 size) {

	ecs->forEach<PerspectiveCamera>([&size](auto& camera) {
		camera.getViewport().setSize(size);
	});

	ecs->forEach<OrthographicCamera>([&size](auto& camera) {
		camera.getViewport().setSize(size);
	});
}


void Scene::addCriticalSystems() {

	// Transform system: updates transform components when they're modified
	ecs->addSystem<TransformSystem>();

	// Camera system: updates the buffers of camera components
	ecs->addSystem<CameraSystem>();

	// Model system: updates the buffers of model components
	ecs->addSystem<ModelSystem>();
}