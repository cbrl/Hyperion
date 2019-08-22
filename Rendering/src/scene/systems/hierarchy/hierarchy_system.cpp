#include "hierarchy_system.h"
#include "ecs.h"
#include "scene/components/hierarchy/hierarchy.h"


namespace render::systems {

HierarchySystem::HierarchySystem(ecs::ECS& ecs)
	: System(ecs)
	, on_destroy_connection(ecs.getDispatcher<ecs::EntityDestroyed>().addCallback<&HierarchySystem::onEntityDestroyed>(this)) {
}

void HierarchySystem::update() {

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