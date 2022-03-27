module;

#include "memory/handle/handle.h"

export module entities.core_entities;

import ecs;
import components.core_components;


export namespace EntityTemplates {

// Does nothing to the specified entity
struct EmptyT final {
	void operator()(ecs::ECS& ecs, handle64 entity) {};
};


// Adds a transform component to the specified entity. The basic
// template for any non-specific entity with a position in the world.
struct WorldObjectT {
	void operator()(ecs::ECS& ecs, handle64 entity) {
		ecs.add<Transform>(entity);
	}
};


// A basic entity with a transform and a hierarchy.
struct HierarchyT : private WorldObjectT {
	void operator()(ecs::ECS& ecs, handle64 entity) {
		WorldObjectT::operator()(ecs, entity);
		ecs.add<Hierarchy>(entity);
	}
};

} // namespace EntityTemplates
