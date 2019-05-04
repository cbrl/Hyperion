#include "transform_system.h"
#include "engine/engine.h"


namespace render::systems {

void TransformSystem::registerCallbacks() {
	registerEventCallback(&TransformSystem::onParentChanged);
}

void TransformSystem::onParentChanged(const ecs::Entity::ParentChangedEvent& event) {
	if (auto* transform = event.entity->getComponent<Transform>()) {
		transform->setNeedsUpdate();
		transform->update();
	}
}

} //namespace render::systems