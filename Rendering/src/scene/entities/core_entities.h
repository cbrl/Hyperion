#pragma once

#include "entity/entity.h"
#include "scene/components/core_components.h"


namespace EntityTemplates {

// Does nothing to the specified entity
struct EmptyT final {
	void operator()(Entity& entity) {};
};


// Adds a transform component to the specified entity. The basic
// template for any non-specific entity with a position in the world.
struct WorldObjectT final {
	void operator()(Entity& entity) {
		entity.addComponent<Transform>();
	}
};

} // namespace EntityTemplates