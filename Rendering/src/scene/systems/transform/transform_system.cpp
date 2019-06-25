#include "transform_system.h"
#include "engine/engine.h"


namespace render::systems {

void TransformSystem::registerCallbacks() {
	registerEventCallback(&TransformSystem::onParentChanged);
}

void TransformSystem::update() {
	auto& ecs = this->getECS();

	ecs.forEach<Transform>([this, &ecs](ecs::Entity& entity) {
		auto& transform = entity.getComponent<Transform>();
		updateWorld(transform);
	});
}

void TransformSystem::updateWorld(Transform& transform) {
	auto& ecs = this->getECS();

	auto& owner = ecs.getEntity(transform.getOwner());
	auto* parent_transform = ecs.tryGetComponent<Transform>(owner.getParent());

	if (parent_transform) {
		if (parent_transform->needs_update) return; //early return if the parent transform also needs an update
		auto m = parent_transform->getObjectToWorldMatrix();
		transform.updateMatrix(&m);
		//transform.world = CalculateWorld(transform) * parent_transform->world;
	}
	else {
		transform.updateMatrix();
	}

	transform.needs_update = false;
}

void TransformSystem::onParentChanged(const ecs::Entity::ParentChangedEvent& event) {
	if (auto* transform = this->getECS().tryGetComponent<Transform>(event.entity)) {
		transform->setNeedsUpdate();
	}
}

} //namespace render::systems