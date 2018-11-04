#include "scene.h"


void Scene::onResize(vec2_u32 size) {

	ecs->forEach<PerspectiveCamera>([&size](auto& camera) {
		camera.getViewport().setSize(size);
	});

	ecs->forEach<OrthographicCamera>([&size](auto& camera) {
		camera.getViewport().setSize(size);
	});
}


EntityPtr Scene::importModel(ID3D11Device& device, const shared_ptr<ModelBlueprint>& blueprint) {

	EntityPtr out = addEntity();
	importModel(out, device, blueprint);
	return out;
}


void Scene::importModel(EntityPtr entity, ID3D11Device& device, const shared_ptr<ModelBlueprint>& blueprint) {

	const auto& materials = blueprint->getMaterials();

	EntityPtr root = addEntity<WorldObject>();
	blueprint->forEachPart([&](ModelPart& part) {
		root->addComponent<Model>(device, blueprint->mesh, part, materials[part.material_index]);
	});

	entity->addChild(root);
}


void Scene::addCriticalSystems() {

	// Transform system: updates transform components when they're modified
	ecs->addSystem<TransformSystem>();

	// Camera system: updates the buffers of camera components
	ecs->addSystem<CameraSystem>();

	// Model system: updates the buffers of model components
	ecs->addSystem<ModelSystem>();
}