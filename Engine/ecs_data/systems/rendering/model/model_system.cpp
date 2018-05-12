#include "stdafx.h"
#include "model_system.h"
#include "ecs\ecs.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\model\model.h"


void ModelSystem::Update(const Engine& engine) {

	ECS::Get()->ForEachActive<Model>([&](Model& model) {

		const auto owner = model.GetOwner();
		const auto transform = ECS::Get()->GetComponent<Transform>(owner);

		if (transform) {
			if (transform->Updated()) {
				model.UpdateBoundingVolumes(transform->GetWorld());
			}
		}
	});
}
