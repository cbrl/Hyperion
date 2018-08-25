#include "scene.h"


void Scene::onResize(vec2_u32 size) {

	ecs->forEach<PerspectiveCamera>([&size](auto& camera) {
		camera.getViewport().setSize(size);
	});

	ecs->forEach<OrthographicCamera>([&size](auto& camera) {
		camera.getViewport().setSize(size);
	});
}


handle64 Scene::importModel(ID3D11Device& device, shared_ptr<ModelBlueprint>& blueprint) {

	handle64 out = addEntity();
	importModel(out, device, blueprint);
	return out;
}


void Scene::importModel(handle64 entity, ID3D11Device& device, shared_ptr<ModelBlueprint>& blueprint) {

	const auto& materials = blueprint->getMaterials();

	blueprint->forEachPart([&](ModelPart& part) {
		ecs->addComponent<Model>(entity, device, blueprint->mesh, part, materials[part.material_index]);
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