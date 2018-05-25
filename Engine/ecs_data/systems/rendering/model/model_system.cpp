#include "stdafx.h"
#include "model_system.h"
#include "engine\engine.h"
#include "ecs_data\components\geometry\transform\transform.h"
#include "ecs_data\components\rendering\model\model.h"


void ModelSystem::Update(const Engine& engine) {
	auto& ecs_engine = engine.GetECS();
	auto& device_context = engine.GetRenderingMgr().GetDeviceContext();

	ecs_engine.ForEachActive<Model>([&](Model& model) {

		const auto owner = model.GetOwner();
		const auto transform = ecs_engine.GetComponent<Transform>(owner);

		if (transform) {
			if (transform->Updated()) {
				// Update the model's bounding volumes
				model.UpdateBoundingVolumes(transform->GetObjectToWorldMatrix());
			}

			// Update the model's buffer
			model.UpdateBuffer(device_context, transform->GetObjectToWorldMatrix());
		}
	});
}
