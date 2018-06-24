#include "transform_system.h"
#include "engine/engine.h"


void TransformSystem::update(const Engine& engine) {
	auto& ecs_engine = engine.getECS();

	ecs_engine.forEachActive<Transform>([&](Transform& transform) {
		updateWorld(ecs_engine, transform);
	});
}


void TransformSystem::postUpdate(const Engine& engine) {
	auto& ecs_engine = engine.getECS();

	ecs_engine.forEachActive<Transform>([&](Transform& transform) {
		transform.updated = false;
	});
}


void TransformSystem::updateWorld(ECS& ecs_engine, Transform& transform) {

	XMMATRIX parent_world = XMMatrixIdentity();

	// If the transform has no parent and doesn't need an update,
	// then nothing needs to be done.
	if (transform.getParent() == handle64::invalid_handle) {
		if (!transform.needs_update) return;
	}

	// If the transform has a parent, then process that first.
	// If the parent was updated, or if this transform already needs
	// an update, then get the parent's matrix.
	else {
		const auto parent = ecs_engine.getComponent<Transform>(transform.getParent());

		if (parent) {
			updateWorld(ecs_engine, *parent);

			if (parent->updated)
				transform.needs_update = true;

			// This may be true even if the above statement is not
			if (transform.needs_update)
				parent_world = parent->getObjectToWorldMatrix();
		}
	}


	if (transform.needs_update) {
		const XMMATRIX this_world = calculateWorld(transform);
		transform.world = this_world * parent_world;

		transform.needs_update = false;
		transform.updated = true;
	}
}


XMMATRIX TransformSystem::calculateWorld(Transform& transform) {
	return XMMatrixScalingFromVector(transform.scaling)
	       * XMMatrixRotationRollPitchYawFromVector(transform.rotation)
	       * XMMatrixTranslationFromVector(transform.translation);
}