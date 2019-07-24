#include "hierarchy_system.h"
#include "ecs.h"
#include "scene/components/hierarchy/hierarchy.h"


namespace render::systems {

void HierarchySystem::update() {

}


void HierarchySystem::registerCallbacks() {
	this->registerEventCallback(&HierarchySystem::onEntityDestroyed);
}


void HierarchySystem::onEntityDestroyed(const ecs::EntityDestroyed& event) {
	auto& ecs = this->getECS();

	if (auto* hierarchy = ecs.tryGet<Hierarchy>(event.entity)) {
		hierarchy->forEachChild(ecs, [&ecs](handle64 child) {
			ecs.destroy(child);
		});
	}
}


} //namespace render::systems