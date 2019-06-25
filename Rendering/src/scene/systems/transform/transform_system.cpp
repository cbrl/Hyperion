#include "transform_system.h"
#include "engine/engine.h"


namespace render::systems {

void TransformSystem::registerCallbacks() {
	registerEventCallback(&TransformSystem::onParentChanged);
}

void TransformSystem::update() {
	auto& ecs = this->getECS();

	// Set update flags of child transforms
	ecs.forEach<Transform>([this, &ecs](ecs::Entity& entity) {
		auto& transform = entity.getComponent<Transform>();
		
		if (transform.needs_update) {
			entity.forEachChildRecursive([](ecs::Entity& child) {
				if (auto* child_transform = child.tryGetComponent<Transform>()) {
					child_transform->setNeedsUpdate();
				}
			});
		}
	});

	// Update all transforms
	ecs.forEach<Transform>([this, &ecs](ecs::Entity& entity) {
		auto& transform = entity.getComponent<Transform>();
		if (not transform.needs_update) {
			return;
		}

		const bool updated = updateWorld(transform);
		if (not updated) {
			return;
		}

		// Update all children if their parent doesn't need an update
		entity.forEachChildRecursive([this](ecs::Entity& child) {
			if (auto* transform = child.tryGetComponent<Transform>()) {
				updateWorld(*transform);
			}
		});
	});
}

bool TransformSystem::updateWorld(Transform& transform) {
	auto& ecs = this->getECS();

	auto& owner = ecs.getEntity(transform.getOwner());
	auto* parent_transform = ecs.tryGetComponent<Transform>(owner.getParent());

	if (parent_transform) {
		if (parent_transform->needs_update)
			return false; //early return if the parent transform also needs an update

		auto m = parent_transform->getObjectToWorldMatrix();
		transform.update(&m);
	}
	else {
		transform.update();
	}

	transform.needs_update = false;
	return true;
}

void TransformSystem::onParentChanged(const ecs::Entity::ParentChangedEvent& event) {
	if (auto* transform = this->getECS().tryGetComponent<Transform>(event.entity)) {
		transform->setNeedsUpdate();
	}
}

} //namespace render::systems