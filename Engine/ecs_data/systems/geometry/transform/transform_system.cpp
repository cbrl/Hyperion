#include "stdafx.h"
#include "transform_system.h"
#include "engine\engine.h"


void TransformSystem::Update(const Engine& engine) {

	ECS::Get()->ForEach<Transform>([&](Transform& transform) {
		UpdateWorld(transform);
	});

	ECS::Get()->ForEach<CameraTransform>([&](CameraTransform& transform) {
		UpdateWorld(transform);
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
