#include "stdafx.h"
#include "transform_system.h"


void TransformSystem::Update(float dt) {

	ECS::Get()->ForEach<Transform>([&](Transform& transform) {
		UpdateWorld(transform);
	});

	ECS::Get()->ForEach<CameraTransform>([&](CameraTransform& transform) {
		UpdateWorld(transform);
	});
}


void TransformSystem::PostUpdate(float dt) {

	ECS::Get()->ForEach<Transform>([&](Transform& transform) {
		transform.updated = false;
	});

	ECS::Get()->ForEach<CameraTransform>([&](CameraTransform& transform) {
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
	
	return rotation * XMMatrixTranslationFromVector(transform.GetPosition());
}
