#pragma once

#include "entity/entity.h"
#include "scene/components/core_components.h"


namespace EntityTemplates {

// Does nothing to the specified entity
struct EmptyT final {
	static void applyTemplate(Entity& entity) {};
};


// Adds a transform component to the specified entity. The basic
// template for any non-specific entity with a position in the world.
struct WorldObjectT final {
	static void applyTemplate(Entity& entity) {
		entity.addComponent<Transform>();
	}
};


// Adds a model to the specified entity
struct ModelT final {
	static void applyTemplate(Entity& entity, ID3D11Device& device, const std::shared_ptr<ModelBlueprint>& bp) {
		entity.addComponent<Transform>();
		entity.addComponent<ModelRoot>(device, bp);
	}
};

} // namespace EntityTemplates