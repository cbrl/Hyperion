#include "transform_system.h"
#include "scene/components/transform/transform.h"
#include "engine/engine.h"


namespace render::systems {

void TransformSystem::registerCallbacks() {
	registerEventCallback(&TransformSystem::onParentChanged);
}

void TransformSystem::update() {
	auto& ecs = this->getECS();

	// Set update flags of child transforms
	ecs.forEach<Transform, Hierarchy>([this, &ecs](handle64 entity) {
		auto& transform = ecs.get<Transform>(entity);
		auto& hierarchy = ecs.get<Hierarchy>(entity);
		
		if (transform.needs_update) {
			hierarchy.forEachChildRecursive(ecs, [&ecs](handle64 child) {
				if (auto* child_transform = ecs.tryGet<Transform>(child)) {
					child_transform->setNeedsUpdate();
				}
			});
		}
	});

	// Update all transforms
	ecs.forEach<Transform>([this, &ecs](handle64 entity) {
		auto& transform = ecs.get<Transform>(entity);
		if (not transform.needs_update) {
			return;
		}

		const bool updated = updateWorld(transform);
		if (not updated) {
			return;
		}

		// Update children if their parent doesn't need an update
		if (auto* hierarchy = ecs.tryGet<Hierarchy>(entity)) {
			hierarchy->forEachChildRecursive(ecs, [this, &ecs](handle64 child) {
				if (auto* transform = ecs.tryGet<Transform>(child)) {
					updateWorld(*transform);
				}
			});
		}
	});
}

bool TransformSystem::updateWorld(Transform& transform) {
	auto& ecs = this->getECS();

	if (auto* hierarchy = ecs.tryGet<Hierarchy>(transform.getOwner());
		hierarchy && ecs.has<Transform>(hierarchy->getParent())) {

		auto& parent_transform = ecs.get<Transform>(hierarchy->getParent());

		if (parent_transform.needs_update)
			return false; //early return if the parent transform also needs an update

		auto m = parent_transform.getObjectToWorldMatrix();
		transform.update(&m);
	}
	else {
		transform.update();
	}

	transform.needs_update = false;
	return true;
}

void TransformSystem::onParentChanged(const Hierarchy::ParentChangedEvent& event) {
	if (auto* transform = this->getECS().tryGet<Transform>(event.entity)) {
		transform->setNeedsUpdate();
	}
}

} //namespace render::systems