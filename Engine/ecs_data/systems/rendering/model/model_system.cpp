#include "stdafx.h"
#include "model_system.h"
#include "engine\engine.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\model\model.h"


void ModelSystem::Update(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();

	ecs_engine.ForEachActive<Model>([&](Model& model) {

		const auto owner = model.GetOwner();
		const auto transform = ecs_engine.GetComponent<Transform>(owner);

		if (transform) {
			if (transform->Updated()) {
				model.UpdateBoundingVolumes(transform->GetWorld());
			}
		}
	});
}
