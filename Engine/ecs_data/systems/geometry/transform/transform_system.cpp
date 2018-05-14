#include "stdafx.h"
#include "transform_system.h"
#include "engine\engine.h"


void TransformSystem::Update(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();

	ecs_engine.ForEachActive<Transform>([&](Transform& transform) {
		UpdateWorld(ecs_engine, transform);
	});

	ecs_engine.ForEachActive<CameraTransform>([&](CameraTransform& transform) {
		UpdateWorld(ecs_engine, transform);
	});
}


void TransformSystem::PostUpdate(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();

	ecs_engine.ForEachActive<Transform>([&](Transform& transform) {
		transform.updated = false;
	});
	ecs_engine.ForEachActive<CameraTransform>([&](CameraTransform& transform) {
		transform.updated = false;
	});
}


XMMATRIX TransformSystem::CalculateWorld(Transform& transform) const {
	return XMMatrixScalingFromVector(transform.scale)
	       * XMMatrixRotationRollPitchYawFromVector(transform.rotation)
	       * XMMatrixTranslationFromVector(transform.translation);
}


XMMATRIX TransformSystem::CalculateWorld(CameraTransform& transform) const {
	XMMATRIX rotation = { transform.GetAxisX(),
		                  transform.GetAxisY(),
		                  transform.GetAxisZ(),
		                  {0.0f, 0.0f, 0.0f, 1.0f} };
	
	return rotation * transform.GetPositionMatrix();
}
