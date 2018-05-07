#include "stdafx.h"
#include "transform_system.h"


void TransformSystem::Update(float dt) {

	ECS::Get()->ForEach<Transform>([&](Transform& transform) {
		UpdateWorld(transform);
	});
}


void TransformSystem::PostUpdate(float dt) {

	ECS::Get()->ForEach<Transform>([&](Transform& transform) {
		if (transform.needs_update) {
			transform.needs_update = false;
			transform.updated      = true;
		}
		else {
			transform.updated = false;
		}
	});
}


void TransformSystem::UpdateWorld(Transform& transform) {

	// Don't update if the object hasn't been transformed
	if (!transform.needs_update) return;


	// Calculate the new object-to-world matrix
	XMMATRIX world = XMMatrixScalingFromVector(transform.scale)
	                 * XMMatrixRotationRollPitchYawFromVector(transform.rotation)
	                 * XMMatrixTranslationFromVector(transform.translation);

	// Set the new matrix
	transform.world = world;
}
