#pragma once

#include "entity/entity.h"
#include "scene/components/transform/transform.h"


namespace EntityTemplates {

// Does nothing to the specified entity
struct Empty final {
	static void applyTemplate(Entity& entity) {};
};


// Adds a transform component to the specified entity. The basic
// template for any non-specific entity with a position in the world.
struct WorldObject final {
	static void applyTemplate(Entity& entity) {
		entity.addComponent<Transform>();
	}
};

} // namespace EntityTemplates