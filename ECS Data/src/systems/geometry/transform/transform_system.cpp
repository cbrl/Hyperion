#include "transform_system.h"
#include "engine/engine.h"


void TransformSystem::update(const Engine& engine) {
	auto& ecs_engine = engine.getECS();

	ecs_engine.forEachActive<Transform>([&](Transform& transform) {
		updateWorld(ecs_engine, transform);
	});

	ecs_engine.forEachActive<CameraTransform>([&](CameraTransform& transform) {
		updateWorld(ecs_engine, transform);
	});
}


void TransformSystem::postUpdate(const Engine& engine) {
	auto& ecs_engine = engine.getECS();

	ecs_engine.forEachActive<Transform>([&](Transform& transform) {
		transform.updated = false;
	});
	ecs_engine.forEachActive<CameraTransform>([&](CameraTransform& transform) {
		transform.updated = false;
	});
}


XMMATRIX TransformSystem::calculateWorld(Transform& transform) {
	return XMMatrixScalingFromVector(transform.scaling)
	       * XMMatrixRotationRollPitchYawFromVector(transform.rotation)
	       * XMMatrixTranslationFromVector(transform.translation);
}


XMMATRIX TransformSystem::calculateWorld(CameraTransform& transform) {
	const XMMATRIX rotation = {
		transform.getWorldAxisX(),
		transform.getWorldAxisY(),
		transform.getWorldAxisZ(),
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	return rotation * transform.getObjectToParentPositionMatrix();
}
