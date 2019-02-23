#include "transform_system.h"
#include "engine/engine.h"


XMMATRIX XM_CALLCONV CalculateWorld(Transform& transform) {
	return XMMatrixScalingFromVector(transform.getScale())
	       * XMMatrixRotationRollPitchYawFromVector(transform.getRotation())
	       * XMMatrixTranslationFromVector(transform.getPosition());
}


void TransformSystem::registerCallbacks() {
	registerEventCallback(&TransformSystem::onTransformNeedsUpdate);
	registerEventCallback(&TransformSystem::onParentChanged);
}


void TransformSystem::updateWorld(Transform& transform) {

	auto* owner  = transform.getOwner().get();
	auto* parent = owner->getParent().get();

	Transform* parent_transform = parent ? parent->getComponent<Transform>() : nullptr;

	if (parent_transform) {
		if (parent_transform->needs_update) return; //early return if the parent transform also needs an update
		transform.world = CalculateWorld(transform) * parent_transform->world;
	}
	else {
		transform.world = CalculateWorld(transform);
	}

	transform.needs_update = false;
	sendEvent<TransformUpdated>(transform);
}


void TransformSystem::onTransformNeedsUpdate(const TransformNeedsUpdate& event) {
	
	auto& transform = event.transform.get();
	auto* owner     = transform.getOwner().get();
	auto* parent    = owner->getParent().get();

	// Early return if the transform has a parent and it needs an update too
	if (parent) {
		if (auto* parent_transform = parent->getComponent<Transform>()) {
			if (parent_transform->needs_update) return;
		}
	}

	updateWorld(transform);

	owner->forEachChild([&](EntityPtr child) {
		if (auto* transform = child->getComponent<Transform>()) {
			transform->sendNeedsUpdateEvent();
		}
	});
}


void TransformSystem::onParentChanged(const ParentChanged& event) {
	if (auto* transform = event.entity->getComponent<Transform>()) {
		transform->sendNeedsUpdateEvent();
	}
}