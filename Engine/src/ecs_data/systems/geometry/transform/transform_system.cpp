#include "stdafx.h"
#include "transform_system.h"
#include "engine\engine.h"


void TransformSystem::update(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();

	ecs_engine.forEachActive<Transform>([&](Transform& transform) {
		UpdateWorld(ecs_engine, transform);
	});

	ecs_engine.forEachActive<CameraTransform>([&](CameraTransform& transform) {
		UpdateWorld(ecs_engine, transform);
	});
}


void TransformSystem::postUpdate(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();

	ecs_engine.forEachActive<Transform>([&](Transform& transform) {
		transform.updated = false;
	});
	ecs_engine.forEachActive<CameraTransform>([&](CameraTransform& transform) {
		transform.updated = false;
	});
}


const XMMATRIX TransformSystem::CalculateWorld(Transform& transform) const {
	return XMMatrixScalingFromVector(transform.scale)
	       * XMMatrixRotationRollPitchYawFromVector(transform.rotation)
	       * XMMatrixTranslationFromVector(transform.translation);
}


const XMMATRIX TransformSystem::CalculateWorld(CameraTransform& transform) const {
	XMMATRIX rotation = { transform.GetWorldAxisX(),
		                  transform.GetWorldAxisY(),
		                  transform.GetWorldAxisZ(),
		                  {0.0f, 0.0f, 0.0f, 1.0f} };
	
	return rotation * transform.GetObjectToParentPositionMatrix();
}
